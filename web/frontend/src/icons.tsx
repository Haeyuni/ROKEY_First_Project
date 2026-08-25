// 라인 스타일 아이콘 모음. 이모지 대신 SVG로 통일해 색/크기를 자유롭게 조절한다.
interface IconProps {
  size?: number;
  color?: string;
}

export function ChevronDownIcon({ size = 14, color = "#8B8398" }: IconProps) {
  return (
    <svg width={size} height={size} viewBox="0 0 24 24" fill="none">
      <path d="M6 9l6 6 6-6" stroke={color} strokeWidth="2.4" strokeLinecap="round" strokeLinejoin="round" />
    </svg>
  );
}

export function ArrowRightIcon({ size = 18, color = "#fff" }: IconProps) {
  return (
    <svg width={size} height={size} viewBox="0 0 24 24" fill="none">
      <path d="M9 6l6 6-6 6" stroke={color} strokeWidth="2.6" strokeLinecap="round" strokeLinejoin="round" />
    </svg>
  );
}

export function ArrowLeftIcon({ size = 16, color = "#8C7AE0" }: IconProps) {
  return (
    <svg width={size} height={size} viewBox="0 0 24 24" fill="none">
      <path d="M15 6l-6 6 6 6" stroke={color} strokeWidth="2.6" strokeLinecap="round" strokeLinejoin="round" />
    </svg>
  );
}

export function CheckIcon({ size = 14, color = "#fff" }: IconProps) {
  return (
    <svg width={size} height={size} viewBox="0 0 24 24" fill="none">
      <path d="M4 12l5 5L20 6" stroke={color} strokeWidth="3" strokeLinecap="round" strokeLinejoin="round" />
    </svg>
  );
}

export function LockIcon({ size = 14, color = "#3B3247" }: IconProps) {
  return (
    <svg width={size} height={size} viewBox="0 0 24 24" fill="none">
      <rect x="5" y="10" width="14" height="10" rx="2" stroke={color} strokeWidth="2" />
      <path d="M8 10V7a4 4 0 018 0v3" stroke={color} strokeWidth="2" />
    </svg>
  );
}

export function GearIcon({ size = 20, color = "#8B8398" }: IconProps) {
  return (
    <svg width={size} height={size} viewBox="0 0 24 24" fill="none">
      <path d="M12 15a3 3 0 100-6 3 3 0 000 6z" stroke={color} strokeWidth="2" />
      <path
        d="M19.4 13a7.6 7.6 0 000-2l2-1.5-2-3.5-2.3.9a7.5 7.5 0 00-1.7-1L15 3h-4l-.4 2.9a7.5 7.5 0 00-1.7 1l-2.3-.9-2 3.5L6.6 11a7.6 7.6 0 000 2l-2 1.5 2 3.5 2.3-.9c.5.4 1.1.8 1.7 1L11 21h4l.4-2.9c.6-.2 1.2-.6 1.7-1l2.3.9 2-3.5-2-1.5z"
        stroke={color}
        strokeWidth="1.6"
      />
    </svg>
  );
}

export function SandIcon({ size = 24, color = "currentColor" }: IconProps) {
  return (
    <svg width={size} height={size} viewBox="0 0 24 24" fill="none">
      <rect x="4" y="4" width="16" height="16" rx="3" fill="none" stroke={color} strokeWidth="2" />
      <path d="M7 17L17 7M7 12l5-5M12 17l5-5" stroke={color} strokeWidth="1.6" strokeLinecap="round" />
    </svg>
  );
}

export function BrushIcon({ size = 24, color = "currentColor" }: IconProps) {
  return (
    <svg width={size} height={size} viewBox="0 0 24 24" fill="none">
      <rect x="9" y="2" width="4" height="12" rx="1.5" fill={color} transform="rotate(35 11 8)" />
      <path d="M14 14l4 4-2 2-4-4z" fill={color} />
    </svg>
  );
}

export function DropletIcon({ size = 24, color = "currentColor" }: IconProps) {
  return (
    <svg width={size} height={size} viewBox="0 0 24 24" fill={color}>
      <path d="M12 2C9 7 5 11 5 15a7 7 0 0014 0c0-4-4-8-7-13z" />
    </svg>
  );
}

export function SunIcon({ size = 24, color = "currentColor" }: IconProps) {
  return (
    <svg width={size} height={size} viewBox="0 0 24 24" fill="none" stroke={color} strokeWidth="1.8">
      <circle cx="12" cy="12" r="4" />
      <path
        d="M12 2v3M12 19v3M2 12h3M19 12h3M4.6 4.6l2.1 2.1M17.3 17.3l2.1 2.1M4.6 19.4l2.1-2.1M17.3 6.7l2.1-2.1"
        strokeLinecap="round"
      />
    </svg>
  );
}

export function GemIcon({ size = 24, color = "currentColor" }: IconProps) {
  return (
    <svg width={size} height={size} viewBox="0 0 24 24" fill={color}>
      <path d="M6 3h12l3 5-9 12L3 8z" />
    </svg>
  );
}

export function CloseIcon({ size = 13, color = "#3B3247" }: IconProps) {
  return (
    <svg width={size} height={size} viewBox="0 0 24 24" fill="none">
      <path d="M6 6l12 12M18 6L6 18" stroke={color} strokeWidth="2.6" strokeLinecap="round" />
    </svg>
  );
}

export const STAGE_ICONS: Record<string, (props: IconProps) => JSX.Element> = {
  SAND: SandIcon,
  BRUSH: BrushIcon,
  COAT: DropletIcon,
  CURE: SunIcon,
  STONE: GemIcon,
};
