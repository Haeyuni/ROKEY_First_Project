import { useEffect, useRef } from "react";
import type { ForceSample } from "../types";

// NFR-03: "힘 데이터 링 버퍼 600점 고정" — 20Hz 소스이므로 600점 = 정확히
// 30초. O2(힘 그래프 표시 구간, web.md §10 미결사항)는 이 고정 버퍼 크기로
// 자연히 "최근 30초"로 확정된다. 고정 크기 TypedArray라 세션이 30분
// 이어져도 배열이 자라지 않는다(메모리 누수 방지).
const BUFFER_SIZE = 600;

interface Props {
  latest: ForceSample | null;
}

// FR-15: 실시간 접촉력 그래프. StiffnessHeatmap과 같은 패턴 — React state가
// 아니라 캔버스 + ref로 직접 그려서 20Hz 갱신에도 React 트리 전체가 흔들리지
// 않게 한다.
export function ForceGraph({ latest }: Props) {
  const canvasRef = useRef<HTMLCanvasElement | null>(null);
  const bufferRef = useRef<Float32Array>(new Float32Array(BUFFER_SIZE));
  const writeIndexRef = useRef(0);
  const filledRef = useRef(0);

  useEffect(() => {
    if (!latest) return;

    const buf = bufferRef.current;
    buf[writeIndexRef.current] = latest.fz_n;
    writeIndexRef.current = (writeIndexRef.current + 1) % BUFFER_SIZE;
    filledRef.current = Math.min(filledRef.current + 1, BUFFER_SIZE);

    const canvas = canvasRef.current;
    const ctx = canvas?.getContext("2d");
    if (!canvas || !ctx) return;

    const { width, height } = canvas;
    const n = filledRef.current;
    ctx.clearRect(0, 0, width, height);
    if (n < 2) return;

    const start = (writeIndexRef.current - n + BUFFER_SIZE) % BUFFER_SIZE;
    let min = Infinity;
    let max = -Infinity;
    for (let i = 0; i < n; i++) {
      const v = buf[(start + i) % BUFFER_SIZE];
      if (v < min) min = v;
      if (v > max) max = v;
    }
    const pad = Math.max(0.2, (max - min) * 0.1);
    min -= pad;
    max += pad;

    if (min < 0 && max > 0) {
      const zeroY = height - ((0 - min) / (max - min)) * height;
      ctx.strokeStyle = "#333";
      ctx.setLineDash([4, 4]);
      ctx.beginPath();
      ctx.moveTo(0, zeroY);
      ctx.lineTo(width, zeroY);
      ctx.stroke();
      ctx.setLineDash([]);
    }

    ctx.strokeStyle = "#4a90d9";
    ctx.lineWidth = 1.5;
    ctx.beginPath();
    for (let i = 0; i < n; i++) {
      const v = buf[(start + i) % BUFFER_SIZE];
      const x = (i / (BUFFER_SIZE - 1)) * width;
      const y = height - ((v - min) / (max - min)) * height;
      if (i === 0) ctx.moveTo(x, y);
      else ctx.lineTo(x, y);
    }
    ctx.stroke();
  }, [latest]);

  return (
    <div className="force-graph">
      <canvas ref={canvasRef} width={480} height={140} className="force-graph__canvas" />
      <div className="force-graph__meta">
        {latest ? `Fz = ${latest.fz_n.toFixed(2)} N` : "데이터 없음"} · 최근 30초
      </div>
    </div>
  );
}
