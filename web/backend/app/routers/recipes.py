"""FR-01: recipes.yaml 목록 조회."""

import yaml
from fastapi import APIRouter

from ..config import settings
from ..schemas import RecipeSummary

router = APIRouter(prefix="/api/recipes", tags=["recipes"])


@router.get("", response_model=list[RecipeSummary])
def list_recipes() -> list[RecipeSummary]:
    with open(settings.recipes_file, encoding="utf-8") as f:
        raw = yaml.safe_load(f) or {}
    return [RecipeSummary(**item) for item in raw.get("recipes", [])]
