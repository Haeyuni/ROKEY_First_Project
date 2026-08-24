import type { Recipe } from "./types";

const API_BASE = import.meta.env.VITE_API_BASE ?? "http://localhost:8000";

export async function fetchRecipes(): Promise<Recipe[]> {
  const res = await fetch(`${API_BASE}/api/recipes`);
  if (!res.ok) throw new Error(`레시피 조회 실패: ${res.status}`);
  return res.json();
}

export interface CreateSessionBody {
  recipe_id: string;
  shape_profile_id: string;
  target_material: string;
  layer_total: number;
  max_rework: number;
  enable_brush: boolean;
  enable_stone: boolean;
}

export async function createSession(
  body: CreateSessionBody,
): Promise<{ session_id: string }> {
  const res = await fetch(`${API_BASE}/api/sessions`, {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify(body),
  });
  if (!res.ok) {
    const detail = await res.json().catch(() => ({}));
    throw new Error(detail.detail ?? `세션 생성 실패: ${res.status}`);
  }
  return res.json();
}

export async function cancelSession(sessionId: string): Promise<void> {
  const res = await fetch(`${API_BASE}/api/sessions/${sessionId}/cancel`, {
    method: "POST",
  });
  if (!res.ok) {
    const detail = await res.json().catch(() => ({}));
    throw new Error(detail.detail ?? `세션 취소 실패: ${res.status}`);
  }
}

export const WS_URL = API_BASE.replace(/^http/, "ws") + "/ws";
