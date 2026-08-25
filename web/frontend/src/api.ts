import type { HealthResponse, Recipe, SessionEvent, SessionListItem } from "./types";

const API_BASE = import.meta.env.VITE_API_BASE ?? "http://localhost:8000";

export async function fetchRecipes(): Promise<Recipe[]> {
  const res = await fetch(`${API_BASE}/api/recipes`);
  if (!res.ok) throw new Error(`레시피 조회 실패: ${res.status}`);
  return res.json();
}

export interface RecipeInput {
  id: string;
  name: string;
  layer_total: number;
  description?: string;
}

export async function createRecipe(body: RecipeInput): Promise<Recipe> {
  const res = await fetch(`${API_BASE}/api/recipes`, {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify(body),
  });
  if (!res.ok) {
    const detail = await res.json().catch(() => ({}));
    throw new Error(detail.detail ?? `레시피 생성 실패: ${res.status}`);
  }
  return res.json();
}

export async function updateRecipe(
  id: string,
  body: Partial<Omit<RecipeInput, "id">>,
): Promise<Recipe> {
  const res = await fetch(`${API_BASE}/api/recipes/${encodeURIComponent(id)}`, {
    method: "PUT",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify(body),
  });
  if (!res.ok) {
    const detail = await res.json().catch(() => ({}));
    throw new Error(detail.detail ?? `레시피 수정 실패: ${res.status}`);
  }
  return res.json();
}

export async function deleteRecipe(id: string): Promise<void> {
  const res = await fetch(`${API_BASE}/api/recipes/${encodeURIComponent(id)}`, {
    method: "DELETE",
  });
  if (!res.ok) {
    const detail = await res.json().catch(() => ({}));
    throw new Error(detail.detail ?? `레시피 삭제 실패: ${res.status}`);
  }
}

export async function fetchSessions(params: {
  limit?: number;
  offset?: number;
  resultCode?: string;
} = {}): Promise<SessionListItem[]> {
  const qs = new URLSearchParams();
  qs.set("limit", String(params.limit ?? 20));
  qs.set("offset", String(params.offset ?? 0));
  if (params.resultCode) qs.set("result_code", params.resultCode);
  const res = await fetch(`${API_BASE}/api/sessions?${qs.toString()}`);
  if (!res.ok) throw new Error(`세션 목록 조회 실패: ${res.status}`);
  return res.json();
}

export async function fetchSessionEvents(sessionId: string): Promise<SessionEvent[]> {
  const res = await fetch(`${API_BASE}/api/sessions/${encodeURIComponent(sessionId)}/events`);
  if (!res.ok) throw new Error(`세션 이벤트 조회 실패: ${res.status}`);
  return res.json();
}

export async function fetchHealth(): Promise<HealthResponse> {
  const res = await fetch(`${API_BASE}/api/health`);
  if (!res.ok) throw new Error(`헬스체크 조회 실패: ${res.status}`);
  return res.json();
}

export interface CreateSessionBody {
  recipe_id: string;
  shape_profile_id: string;
  target_material: string;
  layer_total: number;
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
