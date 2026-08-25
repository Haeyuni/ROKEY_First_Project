import type { ErrorCode } from "../types";
import { SEV_SAFETY } from "../types";
import { translateError } from "../faultMessages";
import { MascotFull } from "../mascot";
import { CloseIcon, LockIcon } from "../icons";

interface Props {
  error: ErrorCode;
  onClose: () => void;
}

// FR-33/35: SEV_WARN/ABORT는 확인 후 닫을 수 있고, SEV_SAFETY는 결함이 해소돼
// last_error가 사라질 때까지 닫기 버튼을 잠근다(기존 session-start__locked와
// 동일한 정책 — 배너 대신 코티가 걱정하는 커스텀 팝업으로 표시).
export function AlertModal({ error, onClose }: Props) {
  const locked = error.severity >= SEV_SAFETY;

  return (
    <div className="modal-scrim">
      <div className="modal-wrap">
        <span className="modal-tag">{locked ? "안전 잠금 · SEV_SAFETY" : "알림"}</span>
        <div className="modal">
          <div className={`modal__head ${locked ? "modal__head--lock" : "modal__head--warn"}`}>
            <div className="head-pad" />
            {locked ? (
              <span className="lock-chip">
                <LockIcon size={13} />
              </span>
            ) : (
              <button type="button" className="close-x" onClick={onClose} aria-label="닫기">
                <CloseIcon size={13} />
              </button>
            )}
          </div>
          <div className="modal__mascot">
            <MascotFull pose="worried" size={118} />
          </div>
          <div className="modal__body">
            <div className="modal__title">{locked ? "안전 결함 감지" : "잠시만요!"}</div>
            <div className="modal__msg">
              {translateError(error.code)}
              {error.detail && (
                <>
                  <br />
                  {error.detail}
                </>
              )}
            </div>
          </div>
          {locked ? (
            <button type="button" className="modal__btn btn-locked" disabled>
              <LockIcon size={14} color="#9C9494" />
              잠금 해제 대기 중
            </button>
          ) : (
            <button type="button" className="modal__btn btn-warn" onClick={onClose}>
              확인
            </button>
          )}
        </div>
      </div>
    </div>
  );
}
