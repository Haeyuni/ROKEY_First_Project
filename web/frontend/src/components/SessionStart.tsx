import { useEffect, useState } from "react";
import { cancelSession, createSession, fetchRecipes } from "../api";
import type { Recipe } from "../types";

const TARGET_MATERIALS = ["silicone_model", "artificial_tip"] as const; // FR-03

interface Props {
  safeToMove: boolean; // FR-31: false면 시작 버튼 비활성화
  activeSessionId: string | null;
}

// FR-01/02의 최소 동작 확인용 스텁. 레시피·형상 선택 UI 전체는 Day2 범위.
export function SessionStart({ safeToMove, activeSessionId }: Props) {
  const [recipes, setRecipes] = useState<Recipe[]>([]);
  const [recipeId, setRecipeId] = useState("");
  const [targetMaterial, setTargetMaterial] = useState<string>(TARGET_MATERIALS[0]);
  const [busy, setBusy] = useState(false);
  const [message, setMessage] = useState<string | null>(null);

  useEffect(() => {
    fetchRecipes()
      .then((list) => {
        setRecipes(list);
        if (list.length > 0) setRecipeId(list[0].id);
      })
      .catch((err) => setMessage(String(err)));
  }, []);

  async function handleStart() {
    setBusy(true);
    setMessage(null);
    try {
      const recipe = recipes.find((r) => r.id === recipeId);
      const { session_id } = await createSession({
        recipe_id: recipeId,
        shape_profile_id: "default",
        target_material: targetMaterial,
        layer_total: recipe?.layer_total ?? 2,
        max_rework: 2,
        enable_brush: true,
        enable_stone: false,
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

  return (
    <div className="session-start">
      <select value={recipeId} onChange={(e) => setRecipeId(e.target.value)} disabled={busy}>
        {recipes.map((r) => (
          <option key={r.id} value={r.id}>
            {r.name}
          </option>
        ))}
      </select>
      <select
        value={targetMaterial}
        onChange={(e) => setTargetMaterial(e.target.value)}
        disabled={busy}
      >
        {TARGET_MATERIALS.map((m) => (
          <option key={m} value={m}>
            {m}
          </option>
        ))}
      </select>
      <button onClick={handleStart} disabled={!safeToMove || busy || activeSessionId !== null}>
        세션 시작
      </button>
      <button onClick={handleCancel} disabled={!activeSessionId || busy}>
        세션 취소
      </button>
      {message && <p className="session-start__message">{message}</p>}
    </div>
  );
}
