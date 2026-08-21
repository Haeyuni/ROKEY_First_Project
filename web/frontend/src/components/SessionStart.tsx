import { memo, useEffect, useState } from "react";
import { cancelSession, createSession, fetchRecipes } from "../api";
import type { Recipe } from "../types";

const TARGET_MATERIALS = ["silicone_model", "artificial_tip"] as const; // FR-03

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
  const [enableStone, setEnableStone] = useState(false);
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
        <label className="session-start__checkbox">
          <input
            type="checkbox"
            checked={enableStone}
            onChange={(e) => setEnableStone(e.target.checked)}
            disabled={disabled}
          />
          스톤 부착
        </label>
      </div>
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
