import { memo, useEffect, useState } from "react";
import { cancelSession, createSession, fetchRecipes } from "../api";
import type { Recipe } from "../types";

const TARGET_MATERIALS = ["silicone_model", "artificial_tip"] as const; // FR-03

// 디자인 선택지 — 백엔드/레시피 데이터에 아직 대응 필드가 없어 UI 상태로만
// 유지한다(선택 결과는 세션 생성 요청에 포함되지 않음).
const DESIGNS = [
  { id: "simple", name: "심플 원톤", description: "단색으로 깔끔하게", swatch: "#caa87a" },
  {
    id: "french",
    name: "프렌치 라인",
    description: "팁 라인을 강조한 디자인",
    swatch: "linear-gradient(to top, #f6e9da 55%, #ffffff 55%)",
  },
  {
    id: "gradient",
    name: "그라데이션",
    description: "두 컬러의 자연스러운 그라데이션",
    swatch: "linear-gradient(135deg, #f2b6c6, #b6d8f2)",
  },
] as const;

// 큐빅 선택지 — "큐빅 없음"이 기존 enable_stone=false에, 나머지 두 종류가
// enable_stone=true에 대응한다(FR-02 큐빅 종류 자체는 백엔드 필드 없음).
const CUBICS = [
  { id: "none", name: "큐빅 없음", description: "장식 없이 깔끔하게", swatch: "#3a4048", stone: false },
  { id: "clear", name: "클리어 큐빅", description: "투명 큐빅으로 포인트", swatch: "#dfe9f0", stone: true },
  { id: "gold", name: "골드 큐빅", description: "골드 큐빅으로 포인트", swatch: "#d4af37", stone: true },
] as const;

interface Props {
  safeToMove: boolean; // FR-31: false면 시작 버튼 비활성화
  locked: boolean; // FR-33: SEV_SAFETY 에러 시 진행 UI 전체 잠금
  activeSessionId: string | null;
}

// FR-01/02: 레시피·소재·형상·레이어 수를 선택해 세션을 생성한다.
export const SessionStart = memo(function SessionStart({
  safeToMove,
  locked,
  activeSessionId,
}: Props) {
  const [recipes, setRecipes] = useState<Recipe[]>([]);
  const [recipeId, setRecipeId] = useState("");
  const [shapeProfileId, setShapeProfileId] = useState("default");
  const [targetMaterial, setTargetMaterial] = useState<string>(TARGET_MATERIALS[0]);
  const [layerTotal, setLayerTotal] = useState(2);
  const [designId, setDesignId] = useState<string>(DESIGNS[0].id);
  const [cubicId, setCubicId] = useState<string>(CUBICS[0].id);
  const [busy, setBusy] = useState(false);
  const [message, setMessage] = useState<string | null>(null);

  useEffect(() => {
    fetchRecipes()
      .then((list) => {
        setRecipes(list);
        if (list.length > 0) {
          setRecipeId(list[0].id);
          setLayerTotal(list[0].layer_total);
        }
      })
      .catch((err) => setMessage(String(err)));
  }, []);

  function handleRecipeChange(id: string) {
    setRecipeId(id);
    const recipe = recipes.find((r) => r.id === id);
    if (recipe) setLayerTotal(recipe.layer_total);
  }

  async function handleStart() {
    setBusy(true);
    setMessage(null);
    try {
      const enableStone = CUBICS.find((c) => c.id === cubicId)?.stone ?? false;
      const { session_id } = await createSession({
        recipe_id: recipeId,
        shape_profile_id: shapeProfileId,
        target_material: targetMaterial,
        layer_total: layerTotal,
        max_rework: 2,
        enable_brush: true,
        enable_stone: enableStone,
      });
      setMessage(`세션 시작됨: ${session_id}`);
    } catch (err) {
      setMessage(String(err));
    } finally {
      setBusy(false);
    }
  }

  async function handleCancel() {
    if (!activeSessionId) return;
    setBusy(true);
    setMessage(null);
    try {
      await cancelSession(activeSessionId);
      setMessage(`취소 요청됨: ${activeSessionId}`);
    } catch (err) {
      setMessage(String(err));
    } finally {
      setBusy(false);
    }
  }

  const disabled = busy || locked;

  return (
    <div className="session-start">
      {locked && (
        <p className="session-start__locked">
          안전 결함(SEV_SAFETY)이 감지되어 조작이 잠겼습니다. 결함 해소 후 다시 시도하세요.
        </p>
      )}
      <div className="session-start__row">
        <label>
          레시피
          <select
            value={recipeId}
            onChange={(e) => handleRecipeChange(e.target.value)}
            disabled={disabled}
          >
            {recipes.map((r) => (
              <option key={r.id} value={r.id}>
                {r.name}
              </option>
            ))}
          </select>
        </label>
        <label>
          소재
          <select
            value={targetMaterial}
            onChange={(e) => setTargetMaterial(e.target.value)}
            disabled={disabled}
          >
            {TARGET_MATERIALS.map((m) => (
              <option key={m} value={m}>
                {m}
              </option>
            ))}
          </select>
        </label>
        <label>
          형상 프로필
          <input
            type="text"
            value={shapeProfileId}
            onChange={(e) => setShapeProfileId(e.target.value)}
            disabled={disabled}
          />
        </label>
        <label>
          레이어 수
          <input
            type="number"
            min={1}
            max={5}
            value={layerTotal}
            onChange={(e) => setLayerTotal(Number(e.target.value))}
            disabled={disabled}
          />
        </label>
      </div>

      <fieldset className="option-group" disabled={disabled}>
        <legend className="option-group__legend">디자인 선택</legend>
        <div className="option-group__grid">
          {DESIGNS.map((d) => (
            <label key={d.id} className="option-card">
              <input
                type="radio"
                name="design-choice"
                value={d.id}
                checked={designId === d.id}
                onChange={() => setDesignId(d.id)}
                disabled={disabled}
              />
              <span className="option-card__swatch" style={{ background: d.swatch }} />
              <span className="option-card__body">
                <span className="option-card__name">{d.name}</span>
                <span className="option-card__desc">{d.description}</span>
              </span>
            </label>
          ))}
        </div>
      </fieldset>

      <fieldset className="option-group" disabled={disabled}>
        <legend className="option-group__legend">큐빅 선택</legend>
        <div className="option-group__grid">
          {CUBICS.map((c) => (
            <label key={c.id} className="option-card">
              <input
                type="radio"
                name="cubic-choice"
                value={c.id}
                checked={cubicId === c.id}
                onChange={() => setCubicId(c.id)}
                disabled={disabled}
              />
              <span className="option-card__swatch option-card__swatch--round" style={{ background: c.swatch }} />
              <span className="option-card__body">
                <span className="option-card__name">{c.name}</span>
                <span className="option-card__desc">{c.description}</span>
              </span>
            </label>
          ))}
        </div>
      </fieldset>

      <div className="session-start__row">
        <button onClick={handleStart} disabled={!safeToMove || disabled || activeSessionId !== null}>
          세션 시작
        </button>
        <button onClick={handleCancel} disabled={!activeSessionId || disabled}>
          세션 취소
        </button>
      </div>
      {message && <p className="session-start__message">{message}</p>}
    </div>
  );
});
