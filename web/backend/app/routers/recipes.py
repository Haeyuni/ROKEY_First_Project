"""FR-01: recipes.yaml 목록 조회 + 관리자 대시보드용 CRUD.

recipes.yaml이 그대로 진실 소스(source of truth)다 — recipe_id는
RunSession.action 필드로는 존재하지만 orchestrator 쪽에서 분기에 쓰지
않는 메타데이터라(layer_total만 실행에 영향), 파일을 자유롭게 편집해도
공정 자체는 안전하다.

동시 쓰기 경쟁은 프로세스 내 asyncio.Lock으로 직렬화한다 — 멀티 워커로
띄우면 안전하지 않지만, 이 도구의 예상 사용 규모(로컬 관리자 1~2명)에는
충분하다.
"""

import asyncio
import os
import tempfile

import yaml
from fastapi import APIRouter, HTTPException

from ..config import settings
from ..schemas import RecipeCreate, RecipeSummary, RecipeUpdate

router = APIRouter(prefix="/api/recipes", tags=["recipes"])

_write_lock = asyncio.Lock()


def _read_all() -> list[dict]:
    with open(settings.recipes_file, encoding="utf-8") as f:
        raw = yaml.safe_load(f) or {}
    return raw.get("recipes", [])


def _write_all(recipes: list[dict]) -> None:
    """임시 파일에 쓴 뒤 os.replace로 교체 — 쓰는 도중 프로세스가 죽어도
    recipes.yaml 자체는 항상 이전 버전이거나 새 버전이지, 반쯤 쓰인
    상태가 되지 않는다."""
    dir_ = os.path.dirname(settings.recipes_file) or "."
    fd, tmp_path = tempfile.mkstemp(dir=dir_, suffix=".yaml.tmp")
    try:
        with os.fdopen(fd, "w", encoding="utf-8") as f:
            yaml.safe_dump({"recipes": recipes}, f, allow_unicode=True, sort_keys=False)
        os.replace(tmp_path, settings.recipes_file)
    except BaseException:
        os.unlink(tmp_path)
        raise


@router.get("", response_model=list[RecipeSummary])
def list_recipes() -> list[RecipeSummary]:
    return [RecipeSummary(**item) for item in _read_all()]


@router.post("", response_model=RecipeSummary, status_code=201)
async def create_recipe(body: RecipeCreate) -> RecipeSummary:
    async with _write_lock:
        recipes = _read_all()
        if any(r.get("id") == body.id for r in recipes):
            raise HTTPException(status_code=409, detail=f"이미 존재하는 레시피 id입니다: {body.id}")
        recipes.append(body.model_dump())
        _write_all(recipes)
    return RecipeSummary(**body.model_dump())


@router.put("/{recipe_id}", response_model=RecipeSummary)
async def update_recipe(recipe_id: str, body: RecipeUpdate) -> RecipeSummary:
    async with _write_lock:
        recipes = _read_all()
        for r in recipes:
            if r.get("id") == recipe_id:
                if body.name is not None:
                    r["name"] = body.name
                if body.layer_total is not None:
                    r["layer_total"] = body.layer_total
                if body.description is not None:
                    r["description"] = body.description
                _write_all(recipes)
                return RecipeSummary(**r)
        raise HTTPException(status_code=404, detail="레시피를 찾을 수 없습니다")


@router.delete("/{recipe_id}", status_code=204)
async def delete_recipe(recipe_id: str) -> None:
    async with _write_lock:
        recipes = _read_all()
        remaining = [r for r in recipes if r.get("id") != recipe_id]
        if len(remaining) == len(recipes):
            raise HTTPException(status_code=404, detail="레시피를 찾을 수 없습니다")
        _write_all(remaining)
