import { useEffect, useRef } from "react";
import type { StiffnessMap, StiffnessPoint } from "../types";

// scan_area_x/y_mm(16×13) + scan_margin_mm(2) 기준 여유를 둔 고정 뷰포트.
// (NIS §6.1 파라미터 기본값 참고. 실측 후 필요하면 조정)
const VIEW_X_MM = 12;
const VIEW_Y_MM = 10;

// mock_robot_driver 기본 강성 범위(k_skin=6 ~ k_nail=40 N/mm, NIS §10)에
// 맞춘 잠정 색상 스케일. O1(색상 스케일 절대/상대값 확정)까지는 절대값 고정.
const STIFFNESS_MIN = 0;
const STIFFNESS_MAX = 50;

function stiffnessToColor(k: number): string {
  const t = Math.max(0, Math.min(1, (k - STIFFNESS_MIN) / (STIFFNESS_MAX - STIFFNESS_MIN)));
  const hue = 220 - t * 220; // 저강성(피부 추정) 파랑 → 고강성(손톱 추정) 빨강
  return `hsl(${hue}, 85%, 50%)`;
}

function drawPoint(ctx: CanvasRenderingContext2D, canvas: HTMLCanvasElement, p: StiffnessPoint): void {
  const px = ((p.position.x + VIEW_X_MM) / (2 * VIEW_X_MM)) * canvas.width;
  const py = ((VIEW_Y_MM - p.position.y) / (2 * VIEW_Y_MM)) * canvas.height;

  if (!p.valid) {
    ctx.fillStyle = "#555";
    ctx.beginPath();
    ctx.arc(px, py, 2, 0, Math.PI * 2);
    ctx.fill();
    return;
  }

  // FR-12: coarse/fine을 크기+테두리로 구분.
  const isFine = p.source === "fine";
  const radius = isFine ? 4 : 3;

  ctx.fillStyle = stiffnessToColor(p.stiffness_n_per_mm); // FR-13
  ctx.beginPath();
  ctx.arc(px, py, radius, 0, Math.PI * 2);
  ctx.fill();

  if (isFine) {
    ctx.strokeStyle = "#fff";
    ctx.lineWidth = 1;
    ctx.stroke();
  }
}

interface Props {
  map: StiffnessMap | null;
}

// FR-11: 스캔 측정 점을 실시간 히트맵으로. NFR-02: 점 추가 시 전체 재렌더
// 없이 증분 렌더링 — 이미 그린 점 개수를 ref로 추적하고 새 점만 그린다.
export function StiffnessHeatmap({ map }: Props) {
  const canvasRef = useRef<HTMLCanvasElement | null>(null);
  const drawnCountRef = useRef(0);
  const sessionRef = useRef<string | null>(null);

  useEffect(() => {
    const canvas = canvasRef.current;
    if (!canvas || !map) return;
    const ctx = canvas.getContext("2d");
    if (!ctx) return;

    if (sessionRef.current !== map.session_id) {
      ctx.clearRect(0, 0, canvas.width, canvas.height);
      drawnCountRef.current = 0;
      sessionRef.current = map.session_id;
    }

    for (const p of map.points.slice(drawnCountRef.current)) {
      drawPoint(ctx, canvas, p);
    }
    drawnCountRef.current = map.points.length;
  }, [map]);

  return (
    <div className="heatmap">
      <canvas ref={canvasRef} width={480} height={400} className="heatmap__canvas" />
      <div className="heatmap__legend">
        <span>
          <i className="legend-dot legend-dot--coarse" /> 거친 스캔 (3mm)
        </span>
        <span>
          <i className="legend-dot legend-dot--fine" /> 정밀 스캔 (1mm)
        </span>
        <span className="legend-scale">
          <i className="legend-swatch" style={{ background: stiffnessToColor(6) }} /> 저강성
          &nbsp;→&nbsp;
          <i className="legend-swatch" style={{ background: stiffnessToColor(40) }} /> 고강성
        </span>
      </div>
      {map && !map.valid && map.points.length > 0 && (
        <div className="heatmap__warning">
          ⚠ 강성 분리도 부족 — 이 스캔 결과는 유효하지 않습니다 (E_SEPARATION_LOW)
        </div>
      )}
    </div>
  );
}
