import { useEffect, useState } from "react";
import { createRecipe, deleteRecipe, fetchRecipes, updateRecipe } from "../api";
import type { Recipe } from "../types";

interface DraftRecipe {
  id: string;
  name: string;
  layer_total: string;
  description: string;
}

const EMPTY_DRAFT: DraftRecipe = { id: "", name: "", layer_total: "2", description: "" };

export function RecipesPanel() {
  const [recipes, setRecipes] = useState<Recipe[] | null>(null);
  const [error, setError] = useState<string | null>(null);
  const [busy, setBusy] = useState(false);

  const [creating, setCreating] = useState(false);
  const [createDraft, setCreateDraft] = useState<DraftRecipe>(EMPTY_DRAFT);

  const [editingId, setEditingId] = useState<string | null>(null);
  const [editDraft, setEditDraft] = useState<DraftRecipe>(EMPTY_DRAFT);

  function load() {
    setError(null);
    fetchRecipes()
      .then(setRecipes)
      .catch((err) => setError(String(err)));
  }

  useEffect(load, []);

  function startCreate() {
    setCreating(true);
    setCreateDraft(EMPTY_DRAFT);
  }

  async function submitCreate() {
    setBusy(true);
    setError(null);
    try {
      await createRecipe({
        id: createDraft.id.trim(),
        name: createDraft.name.trim(),
        layer_total: Number(createDraft.layer_total),
        description: createDraft.description.trim(),
      });
      setCreating(false);
      load();
    } catch (err) {
      setError(String(err));
    } finally {
      setBusy(false);
    }
  }

  function startEdit(r: Recipe) {
    setEditingId(r.id);
    setEditDraft({
      id: r.id,
      name: r.name,
      layer_total: String(r.layer_total),
      description: r.description,
    });
  }

  async function submitEdit() {
    if (!editingId) return;
    setBusy(true);
    setError(null);
    try {
      await updateRecipe(editingId, {
        name: editDraft.name.trim(),
        layer_total: Number(editDraft.layer_total),
        description: editDraft.description.trim(),
      });
      setEditingId(null);
      load();
    } catch (err) {
      setError(String(err));
    } finally {
      setBusy(false);
    }
  }

  async function handleDelete(id: string) {
    if (!window.confirm(`"${id}" 레시피를 삭제할까요? 되돌릴 수 없습니다.`)) return;
    setBusy(true);
    setError(null);
    try {
      await deleteRecipe(id);
      load();
    } catch (err) {
      setError(String(err));
    } finally {
      setBusy(false);
    }
  }

  return (
    <section className="panel">
      <div className="panel__head">
        <div>
          <h2>레시피</h2>
          <p className="panel__desc">recipes.yaml을 직접 읽고 씁니다. layer_total은 세션 생성 시 그대로 반영돼요.</p>
        </div>
        <button type="button" className="btn btn--primary" onClick={startCreate} disabled={creating || busy}>
          + 새 레시피
        </button>
      </div>

      {error && <div className="alert alert--danger">{error}</div>}

      <table className="table">
        <thead>
          <tr>
            <th>ID</th>
            <th>이름</th>
            <th>레이어 수</th>
            <th>설명</th>
            <th aria-label="작업" />
          </tr>
        </thead>
        <tbody>
          {creating && (
            <tr>
              <td>
                <input
                  className="input"
                  value={createDraft.id}
                  onChange={(e) => setCreateDraft({ ...createDraft, id: e.target.value })}
                  placeholder="unique_id"
                  autoFocus
                />
              </td>
              <td>
                <input
                  className="input"
                  value={createDraft.name}
                  onChange={(e) => setCreateDraft({ ...createDraft, name: e.target.value })}
                  placeholder="레시피 이름"
                />
              </td>
              <td>
                <input
                  className="input input--num"
                  type="number"
                  min={1}
                  max={5}
                  value={createDraft.layer_total}
                  onChange={(e) => setCreateDraft({ ...createDraft, layer_total: e.target.value })}
                />
              </td>
              <td>
                <input
                  className="input"
                  value={createDraft.description}
                  onChange={(e) => setCreateDraft({ ...createDraft, description: e.target.value })}
                  placeholder="설명(선택)"
                />
              </td>
              <td className="table__actions">
                <button type="button" className="btn btn--primary btn--sm" onClick={submitCreate} disabled={busy || !createDraft.id.trim() || !createDraft.name.trim()}>
                  저장
                </button>
                <button type="button" className="btn btn--ghost btn--sm" onClick={() => setCreating(false)} disabled={busy}>
                  취소
                </button>
              </td>
            </tr>
          )}

          {recipes === null && (
            <tr>
              <td colSpan={5} className="table__empty">
                불러오는 중...
              </td>
            </tr>
          )}
          {recipes !== null && recipes.length === 0 && !creating && (
            <tr>
              <td colSpan={5} className="table__empty">
                등록된 레시피가 없습니다.
              </td>
            </tr>
          )}

          {recipes?.map((r) =>
            editingId === r.id ? (
              <tr key={r.id}>
                <td className="mono">{r.id}</td>
                <td>
                  <input
                    className="input"
                    value={editDraft.name}
                    onChange={(e) => setEditDraft({ ...editDraft, name: e.target.value })}
                  />
                </td>
                <td>
                  <input
                    className="input input--num"
                    type="number"
                    min={1}
                    max={5}
                    value={editDraft.layer_total}
                    onChange={(e) => setEditDraft({ ...editDraft, layer_total: e.target.value })}
                  />
                </td>
                <td>
                  <input
                    className="input"
                    value={editDraft.description}
                    onChange={(e) => setEditDraft({ ...editDraft, description: e.target.value })}
                  />
                </td>
                <td className="table__actions">
                  <button type="button" className="btn btn--primary btn--sm" onClick={submitEdit} disabled={busy || !editDraft.name.trim()}>
                    저장
                  </button>
                  <button type="button" className="btn btn--ghost btn--sm" onClick={() => setEditingId(null)} disabled={busy}>
                    취소
                  </button>
                </td>
              </tr>
            ) : (
              <tr key={r.id}>
                <td className="mono">{r.id}</td>
                <td>{r.name}</td>
                <td>{r.layer_total}</td>
                <td className="table__muted">{r.description || "—"}</td>
                <td className="table__actions">
                  <button type="button" className="btn btn--ghost btn--sm" onClick={() => startEdit(r)} disabled={busy || creating}>
                    수정
                  </button>
                  <button type="button" className="btn btn--danger-ghost btn--sm" onClick={() => handleDelete(r.id)} disabled={busy || creating}>
                    삭제
                  </button>
                </td>
              </tr>
            ),
          )}
        </tbody>
      </table>
    </section>
  );
}
