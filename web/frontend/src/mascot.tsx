// 마스코트 "코티"와 컬러/파츠 프리뷰 아이콘. 목업(Party Animals풍 캐릭터 선택
// 화면)에서 그대로 옮긴 SVG — 좌표는 손으로 튜닝한 값이라 4/8px 그리드에 맞지
// 않는다.
import type { DesignId, CubicId } from "./options";

interface HeadProps {
  size?: number;
}

export function BrandMark({ size = 34 }: HeadProps) {
  return (
    <svg width={size} height={size} viewBox="0 0 120 140">
      <rect x="38" y="4" width="44" height="20" rx="9" fill="#FFD9C2" />
      <rect x="10" y="22" width="100" height="102" rx="40" fill="#B9E8D3" />
      <circle cx="46" cy="70" r="7" fill="#3B3247" />
      <circle cx="74" cy="70" r="7" fill="#3B3247" />
      <path d="M48 92 Q60 100 72 92" stroke="#3B3247" strokeWidth="5" strokeLinecap="round" fill="none" />
    </svg>
  );
}

interface DesignHeadProps extends HeadProps {
  variant: DesignId;
}

// Step1 카드 스와치 — 색상값은 기존 SessionStart의 DESIGNS 스와치 색을 그대로 재사용.
export function DesignHeadIcon({ variant, size = 108 }: DesignHeadProps) {
  const width = size;
  const height = (size * 140) / 120;

  if (variant === "french") {
    return (
      <svg width={width} height={height} viewBox="0 0 120 140" fill="none">
        <defs>
          <clipPath id="dh-clip-french">
            <rect x="10" y="22" width="100" height="102" rx="40" />
          </clipPath>
        </defs>
        <rect x="38" y="4" width="44" height="20" rx="9" fill="#EDE6DA" stroke="#D9CFC0" strokeWidth="2" />
        <g clipPath="url(#dh-clip-french)">
          <rect x="10" y="22" width="100" height="102" fill="#F6E9DA" />
          <rect x="10" y="22" width="100" height="46" fill="#FFFFFF" />
          <rect x="10" y="66" width="100" height="4" fill="#EADFCB" />
        </g>
        <ellipse cx="40" cy="92" rx="9" ry="5" fill="#FFB199" opacity="0.55" />
        <ellipse cx="80" cy="92" rx="9" ry="5" fill="#FFB199" opacity="0.55" />
        <circle cx="46" cy="76" r="6" fill="#3B3247" />
        <circle cx="74" cy="76" r="6" fill="#3B3247" />
        <path d="M48 96 Q60 106 72 96" stroke="#3B3247" strokeWidth="4" strokeLinecap="round" fill="none" />
      </svg>
    );
  }

  if (variant === "gradient") {
    return (
      <svg width={width} height={height} viewBox="0 0 120 140" fill="none">
        <defs>
          <linearGradient id="dh-grad" x1="15" y1="15" x2="105" y2="115" gradientUnits="userSpaceOnUse">
            <stop offset="0" stopColor="#F2B6C6" />
            <stop offset="1" stopColor="#B6D8F2" />
          </linearGradient>
        </defs>
        <rect x="38" y="4" width="44" height="20" rx="9" fill="#F4F0FA" stroke="#E3DAF2" strokeWidth="2" />
        <rect x="10" y="22" width="100" height="102" rx="40" fill="url(#dh-grad)" />
        <ellipse cx="40" cy="88" rx="9" ry="5" fill="#FFFFFF" opacity="0.35" />
        <ellipse cx="80" cy="88" rx="9" ry="5" fill="#FFFFFF" opacity="0.35" />
        <circle cx="46" cy="70" r="6" fill="#3B3247" />
        <circle cx="74" cy="70" r="6" fill="#3B3247" />
        <path d="M48 92 Q60 102 72 92" stroke="#3B3247" strokeWidth="4" strokeLinecap="round" fill="none" />
      </svg>
    );
  }

  // simple
  return (
    <svg width={width} height={height} viewBox="0 0 120 140" fill="none">
      <rect x="38" y="4" width="44" height="20" rx="9" fill="#EDE6DA" stroke="#D9CFC0" strokeWidth="2" />
      <rect x="10" y="22" width="100" height="102" rx="40" fill="#CAA87A" />
      <ellipse cx="40" cy="88" rx="9" ry="5" fill="#FFB199" opacity="0.55" />
      <ellipse cx="80" cy="88" rx="9" ry="5" fill="#FFB199" opacity="0.55" />
      <circle cx="46" cy="70" r="6" fill="#3B3247" />
      <circle cx="74" cy="70" r="6" fill="#3B3247" />
      <path d="M48 92 Q60 102 72 92" stroke="#3B3247" strokeWidth="4" strokeLinecap="round" fill="none" />
    </svg>
  );
}

interface CubicOverlayProps extends HeadProps {
  variant: CubicId;
}

// Step2 카드용 큐빅 반짝임 오버레이 — DesignHeadIcon 위에 겹쳐 그린다.
// 색상은 기존 SessionStart의 CUBICS 스와치 색(clear=#dfe9f0, gold=#d4af37)을 재사용.
export function CubicOverlayIcon({ variant, size = 108 }: CubicOverlayProps) {
  if (variant === "none") return null;
  const color = variant === "gold" ? "#D4AF37" : "#DFE9F0";
  const width = size;
  const height = (size * 140) / 120;
  return (
    <svg width={width} height={height} viewBox="0 0 120 140" fill="none" style={{ position: "absolute", inset: 0 }}>
      <path d="M24 30 L27 37 L34 40 L27 43 L24 50 L21 43 L14 40 L21 37 Z" fill={color} />
      <path d="M92 46 L94 51 L99 53 L94 55 L92 60 L90 55 L85 53 L90 51 Z" fill={color} />
      <path d="M84 26 L85.5 29.5 L89 31 L85.5 32.5 L84 36 L82.5 32.5 L79 31 L82.5 29.5 Z" fill={color} />
    </svg>
  );
}

// 컬러 + 파츠 조합 프리뷰 (Step2 "지금 이 모습이에요" 카드에서 사용).
export function ColorCubicPreview({
  design,
  cubic,
  size = 108,
}: {
  design: DesignId;
  cubic: CubicId;
  size?: number;
}) {
  const height = (size * 140) / 120;
  return (
    <div style={{ position: "relative", width: size, height }}>
      <DesignHeadIcon variant={design} size={size} />
      <CubicOverlayIcon variant={cubic} size={size} />
    </div>
  );
}

type MascotPose = "working" | "worried" | "celebrating";

// Step3(작업중)/완료 페이지(축하)/경고 팝업(걱정)에 쓰는 전신 마스코트.
export function MascotFull({ pose, size = 132 }: { pose: MascotPose; size?: number }) {
  const width = size;
  const height = (size * 200) / 160;

  if (pose === "worried") {
    return (
      <svg width={width} height={height} viewBox="0 0 160 200">
        <g transform="rotate(14 30 90)">
          <rect x="22" y="90" width="16" height="58" rx="8" fill="#B9E8D3" />
          <circle cx="30" cy="148" r="12" fill="#FFD9C2" />
        </g>
        <g transform="rotate(-70 126 100)">
          <rect x="118" y="46" width="16" height="58" rx="8" fill="#B9E8D3" />
          <circle cx="126" cy="46" r="12" fill="#FFD9C2" />
        </g>
        <rect x="58" y="6" width="44" height="18" rx="8" fill="#FFD9C2" />
        <rect x="28" y="22" width="104" height="120" rx="46" fill="#B9E8D3" />
        <ellipse cx="52" cy="98" rx="9" ry="5" fill="#FFB199" opacity="0.55" />
        <ellipse cx="108" cy="98" rx="9" ry="5" fill="#FFB199" opacity="0.55" />
        <path d="M48 66 L64 72" stroke="#3B3247" strokeWidth="3" strokeLinecap="round" />
        <path d="M112 66 L96 72" stroke="#3B3247" strokeWidth="3" strokeLinecap="round" />
        <circle cx="58" cy="78" r="7" fill="#3B3247" />
        <circle cx="102" cy="78" r="7" fill="#3B3247" />
        <path d="M62 102 Q70 96 78 102 Q86 108 94 102" stroke="#3B3247" strokeWidth="3" strokeLinecap="round" fill="none" />
        <path d="M124 40c6 8 6 14 0 18-6-4-6-10 0-18z" fill="#BFE0FF" />
      </svg>
    );
  }

  if (pose === "celebrating") {
    return (
      <svg width={width} height={height} viewBox="0 0 160 200">
        <g transform="rotate(42 30 100)">
          <rect x="14" y="46" width="16" height="58" rx="8" fill="#B9E8D3" />
          <circle cx="22" cy="46" r="12" fill="#FFD9C2" />
        </g>
        <g transform="rotate(-42 126 100)">
          <rect x="118" y="46" width="16" height="58" rx="8" fill="#B9E8D3" />
          <circle cx="126" cy="46" r="12" fill="#FFD9C2" />
        </g>
        <rect x="58" y="6" width="44" height="18" rx="8" fill="#FFD9C2" />
        <rect x="28" y="22" width="104" height="120" rx="46" fill="#B9E8D3" />
        <ellipse cx="52" cy="98" rx="9" ry="5" fill="#FFB199" opacity="0.55" />
        <ellipse cx="108" cy="98" rx="9" ry="5" fill="#FFB199" opacity="0.55" />
        <path d="M50 76 Q58 68 66 76" stroke="#3B3247" strokeWidth="3" strokeLinecap="round" fill="none" />
        <path d="M94 76 Q102 68 110 76" stroke="#3B3247" strokeWidth="3" strokeLinecap="round" fill="none" />
        <ellipse cx="80" cy="102" rx="10" ry="9" fill="#3B3247" />
        <path d="M14 14 L17 21 L24 24 L17 27 L14 34 L11 27 L4 24 L11 21 Z" fill="#FFC2D6" />
        <path d="M146 24 L148 29 L153 31 L148 33 L146 38 L144 33 L139 31 L144 29 Z" fill="#D9D3F5" />
        <path d="M136 6 L137.5 9.5 L141 11 L137.5 12.5 L136 16 L134.5 12.5 L131 11 L134.5 9.5 Z" fill="#F0C36A" />
      </svg>
    );
  }

  // working
  return (
    <svg width={width} height={height} viewBox="0 0 160 200">
      <g transform="rotate(18 30 90)">
        <rect x="22" y="90" width="16" height="58" rx="8" fill="#B9E8D3" />
        <circle cx="30" cy="148" r="12" fill="#FFD9C2" />
      </g>
      <g transform="rotate(-42 126 100)">
        <rect x="118" y="46" width="16" height="58" rx="8" fill="#B9E8D3" />
        <circle cx="126" cy="46" r="12" fill="#FFD9C2" />
      </g>
      <rect x="58" y="6" width="44" height="18" rx="8" fill="#FFD9C2" />
      <rect x="28" y="22" width="104" height="120" rx="46" fill="#B9E8D3" />
      <ellipse cx="52" cy="96" rx="9" ry="5" fill="#FFB199" opacity="0.55" />
      <ellipse cx="108" cy="96" rx="9" ry="5" fill="#FFB199" opacity="0.55" />
      <circle cx="58" cy="78" r="7" fill="#3B3247" />
      <circle cx="102" cy="78" r="7" fill="#3B3247" />
      <ellipse cx="80" cy="100" rx="6" ry="8" fill="#3B3247" />
      <rect x="150" y="6" width="7" height="24" rx="3" fill="#F2B6C6" transform="rotate(30 153 18)" />
      <path d="M158 16l10 10-6 6-10-10z" fill="#B6D8F2" />
    </svg>
  );
}
