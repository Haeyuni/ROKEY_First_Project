import { Fragment } from "react";
import { CheckIcon } from "../icons";

const LABELS = ["컬러 선택", "파츠 선택", "코팅 진행"];

interface Props {
  current: 1 | 2; // 선택 단계에서만 사용(진행/완료 화면에는 표시하지 않음)
}

export function StepDots({ current }: Props) {
  return (
    <div className="steps-row">
      {LABELS.map((label, i) => {
        const step = i + 1;
        const status = step < current ? "done" : step === current ? "active" : "pending";
        return (
          <Fragment key={label}>
            {i > 0 && <div className="step__line" />}
            <div className={`step step--${status}`}>
              <div className="step__dot">{status === "done" ? <CheckIcon size={13} /> : step}</div>
              <div className="step__label">{label}</div>
            </div>
          </Fragment>
        );
      })}
    </div>
  );
}
