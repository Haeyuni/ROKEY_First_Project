// FR-34, NIS C-E: UV 램프는 상시 ON이고 소프트웨어로 끌 수 없다(SDS §9.3).
// permit 구조가 폐지되었으므로 화면 경고가 안전 통제의 일부다 — 세션 중
// 조건부로 숨기지 않고 항상 노출한다.
export function UvWarningBanner() {
  return (
    <div className="uv-banner" role="alert">
      ⚠ UV 램프는 항상 켜져 있습니다 (소프트웨어로 끌 수 없음) — 차단 고글 착용,
      직접 응시 금지
    </div>
  );
}
