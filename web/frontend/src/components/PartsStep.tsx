import { useState } from "react";
import type { SafetyState } from "../types";
import { CUBICS, TARGET_MATERIALS, cubicName, designName, type CubicId, type DesignId } from "../options";
import { ColorCubicPreview } from "../mascot";
import { ArrowLeftIcon, ArrowRightIcon, CheckIcon, ChevronDownIcon, GearIcon } from "../icons";
import { TopBar } from "./TopBar";
import { StepDots } from "./StepDots";

export interface StartSettings {
  shapeProfileId: string;
  targetMaterial: string;
}

interface Props {
  safety: SafetyState | null;
  connected: boolean;
  design: DesignId;
  cubic: CubicId;
  onChangeCubic: (id: CubicId) => void;
  onBack: () => void;
  onStart: (settings: StartSettings) => void;
  busy: boolean;
  safeToMove: boolean;
  errorMessage: string | null;
}

// Step 2 — 파츠(큐빅) 선택 + 소재/형상/레이어 수 같은 기술 파라미터를
// 담는 접이식 "고급 설정"(기존 SessionStart의 나머지 필드들).
export function PartsStep({
  safety,
  connected,
  design,
  cubic,
  onChangeCubic,
  onBack,
  onStart,
  busy,
  safeToMove,
  errorMessage,
}: Props) {
  const [shapeProfileId, setShapeProfileId] = useState("default");
  const [targetMaterial, setTargetMaterial] = useState<string>(TARGET_MATERIALS[0]);
  const [settingsOpen, setSettingsOpen] = useState(false);

  function handleStart() {
    onStart({ shapeProfileId, targetMaterial });
  }

  return (
    <div className="screen screen--lavender">
      <div className="blob blob--lavender" style={{ top: -120, left: -80 }} />
      <div className="blob blob--peach" style={{ bottom: -110, right: -70, top: "auto" }} />

      <TopBar safety={safety} connected={connected} />
      <StepDots current={2} />

      <div className="layout">
        <div className="left-col">
          <div className="hero hero--left">
            <h1>파츠를 골라주세요</h1>
            <p>큐빅으로 포인트를 줘볼까요?</p>
          </div>

          <div className="cards-row cards-row--parts">
            {CUBICS.map((c) => {
              const selected = c.id === cubic;
              return (
                <button
                  key={c.id}
                  type="button"
                  className="card"
                  style={
                    selected
                      ? { borderColor: "#FF9457", transform: "translateY(-8px)", boxShadow: "0 16px 32px rgba(255,148,87,0.28)" }
                      : undefined
                  }
                  onClick={() => onChangeCubic(c.id)}
                >
                  {selected && (
                    <span className="check-badge">
                      <CheckIcon size={13} />
                    </span>
                  )}
                  <ColorCubicPreview design={design} cubic={c.id} size={96} />
                  <span className="card__name">{c.name}</span>
                  <span className="card__desc">{c.description}</span>
                </button>
              );
            })}
          </div>
        </div>

        <div className="preview-col">
          <div className="preview-col__label">지금 이 모습이에요!</div>
          <ColorCubicPreview design={design} cubic={cubic} size={150} />
          <div className="preview-col__name">
            {designName(design)} · {cubicName(cubic)}
          </div>
        </div>
      </div>

      <button type="button" className="settings-bar" onClick={() => setSettingsOpen((o) => !o)} aria-expanded={settingsOpen}>
        <GearIcon size={20} />
        <span className="settings-bar__text">고급 설정 · 소재 / 형상 프로필</span>
        <ChevronDownIcon size={16} />
      </button>

      {settingsOpen && (
        <div className="settings-panel">
          <label>
            소재
            <select value={targetMaterial} onChange={(e) => setTargetMaterial(e.target.value)} disabled={busy}>
              {TARGET_MATERIALS.map((m) => (
                <option key={m} value={m}>
                  {m}
                </option>
              ))}
            </select>
          </label>
          <label>
            형상 프로필
            <input type="text" value={shapeProfileId} onChange={(e) => setShapeProfileId(e.target.value)} disabled={busy} />
          </label>
        </div>
      )}

      {errorMessage && <p className="parts-step__message">{errorMessage}</p>}

      <div className="screen__footer screen__footer--split">
        <button type="button" className="btn-outline" onClick={onBack} disabled={busy}>
          <ArrowLeftIcon />
          이전
        </button>
        <button type="button" className="btn-fill" onClick={handleStart} disabled={busy || !safeToMove}>
          코팅 시작!
          <ArrowRightIcon />
        </button>
      </div>
    </div>
  );
}
