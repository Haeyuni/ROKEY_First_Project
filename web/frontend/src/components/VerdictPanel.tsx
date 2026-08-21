import { memo } from "react";
import type { ValidationResult } from "../types";

const RESULT_LABEL_KO: Record<string, string> = {
  PASS: "합격",
  FAIL: "불합격",
  SKIP: "생략",
};

interface Props {
  verdicts: ValidationResult[];
}

// FR-20/21: 레이어별 3점(중앙·좌·우) 판정 + 판정 시점 threshold_n 노출.
// FR-22: "손톱 전체가 경화됨"이라고 쓰지 않는다 — 3점 결과라는 한계를 문구로 명시.
export const VerdictPanel = memo(function VerdictPanel({ verdicts }: Props) {
  if (verdicts.length === 0) {
    return <p className="verdict-panel__empty">아직 판정 결과가 없습니다</p>;
  }

  const byLayer = new Map<number, ValidationResult[]>();
  for (const v of verdicts) {
    const list = byLayer.get(v.layer_index) ?? [];
    list.push(v);
    byLayer.set(v.layer_index, list);
  }

  return (
    <div className="verdict-panel">
      {[...byLayer.entries()]
        .sort(([a], [b]) => a - b)
        .map(([layerIndex, points]) => {
          const measured = points.filter((p) => p.result !== "SKIP");
          const allPass = measured.length > 0 && measured.every((p) => p.result === "PASS");
          const anyFail = points.some((p) => p.result === "FAIL");

          return (
            <div key={layerIndex} className="verdict-panel__layer">
              <h3>레이어 {layerIndex + 1}</h3>
              <table className="verdict-table">
                <thead>
                  <tr>
                    <th>측정점</th>
                    <th>결과</th>
                    <th>임계값(N)</th>
                    <th>이탈력(N)</th>
                  </tr>
                </thead>
                <tbody>
                  {points.map((v) => (
                    <tr
                      key={`${v.layer_index}-${v.point_label}`}
                      className={`verdict-table__row verdict-table__row--${v.result.toLowerCase()}`}
                    >
                      <td>{v.point_label}</td>
                      <td>{RESULT_LABEL_KO[v.result] ?? v.result}</td>
                      <td>{v.threshold_n.toFixed(2)}</td>
                      <td>{v.release_force_n.toFixed(2)}</td>
                    </tr>
                  ))}
                </tbody>
              </table>
              {/* FR-22: 화면 문구는 항상 "검사한 N개 지점" 범위로 한정한다. */}
              {allPass && (
                <p className="verdict-panel__caption verdict-panel__caption--ok">
                  검사한 {measured.length}개 지점이 기준을 만족함
                </p>
              )}
              {anyFail && (
                <p className="verdict-panel__caption verdict-panel__caption--fail">
                  검사한 지점 중 일부가 기준을 만족하지 못함 — 재작업 대상
                </p>
              )}
            </div>
          );
        })}
    </div>
  );
});
