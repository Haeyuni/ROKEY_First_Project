import type { ReactNode } from "react";
import type { SafetyState } from "../types";
import { BrandMark } from "../mascot";
import { SafetyPill } from "./SafetyPill";

interface Props {
  safety: SafetyState | null;
  connected: boolean;
  extra?: ReactNode;
}

// 화면 4개(Step1~3, 완료)에서 공통으로 쓰는 상단 바 — 브랜드 마크 + 안전 캡슐은
// 항상 노출, extra로 화면별 부가 컨트롤(취소 버튼 등)을 끼워 넣는다.
export function TopBar({ safety, connected, extra }: Props) {
  return (
    <div className="topbar">
      <div className="brand">
        <BrandMark />
        <span className="brand__name">코티</span>
      </div>
      <div className="topbar__right">
        <SafetyPill safety={safety} connected={connected} />
        {extra}
      </div>
    </div>
  );
}
