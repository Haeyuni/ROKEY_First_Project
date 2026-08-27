import type { SafetyState } from "../types";
import { DESIGNS, type DesignId } from "../options";
import { DesignHeadIcon } from "../mascot";
import { ArrowRightIcon, CheckIcon } from "../icons";
import { TopBar } from "./TopBar";
import { StepDots } from "./StepDots";

interface Props {
  safety: SafetyState | null;
  connected: boolean;
  value: DesignId;
  onChange: (id: DesignId) => void;
  onNext: () => void;
}

// Step 1 — 코팅컬러 선택 (기존 SessionStart의 "디자인 선택" 카드를 재활용).
export function ColorStep({ safety, connected, value, onChange, onNext }: Props) {
  return (
    <div className="screen screen--mint">
      <div className="blob blob--peach" />
      <div className="blob blob--lavender" />

      <TopBar safety={safety} connected={connected} />
      <StepDots current={1} />

      <div className="hero">
        <h1>코팅컬러를 골라주세요</h1>
        <p>코티가 선택한 컬러로 예쁘게 발라드릴게요</p>
      </div>

      <div className="cards-row">
        {DESIGNS.map((d) => {
          const selected = d.id === value;
          return (
            <button
              key={d.id}
              type="button"
              className="card"
              style={
                selected
                  ? { borderColor: "#FF9457", transform: "translateY(-8px)", boxShadow: "0 16px 32px rgba(255,148,87,0.28)" }
                  : undefined
              }
              onClick={() => onChange(d.id)}
            >
              {selected && (
                <span className="check-badge">
                  <CheckIcon size={14} />
                </span>
              )}
              <DesignHeadIcon variant={d.id} />
              <span className="card__name">{d.name}</span>
              <span className="card__desc">{d.description}</span>
            </button>
          );
        })}
      </div>

      <div className="screen__footer">
        <button type="button" className="next-btn" onClick={onNext}>
          다음
          <ArrowRightIcon />
        </button>
      </div>
    </div>
  );
}
