// Step1(코팅컬러)/Step2(파츠) 선택지. 백엔드에는 색상/파츠 자체의 필드가 없어
// enable_stone(파츠 → 스톤 공정 여부)을 제외하면 UI 상태로만 유지한다.

export const DESIGNS = [
  { id: "simple", name: "심플 원톤", description: "단색으로 깔끔하게" },
  { id: "french", name: "프렌치 라인", description: "팁 라인을 강조한 디자인" },
  { id: "gradient", name: "그라데이션", description: "두 컬러의 자연스러운 그라데이션" },
] as const;

export type DesignId = (typeof DESIGNS)[number]["id"];

export const CUBICS = [
  { id: "none", name: "큐빅 없음", description: "장식 없이 깔끔하게", stone: false },
  { id: "clear", name: "클리어 큐빅", description: "투명 큐빅으로 포인트", stone: true },
  { id: "gold", name: "골드 큐빅", description: "골드 큐빅으로 포인트", stone: true },
] as const;

export type CubicId = (typeof CUBICS)[number]["id"];

export const TARGET_MATERIALS = ["silicone_model", "artificial_tip"] as const; // FR-03

export function designName(id: string): string {
  return DESIGNS.find((d) => d.id === id)?.name ?? id;
}

export function cubicName(id: string): string {
  return CUBICS.find((c) => c.id === id)?.name ?? id;
}

export function isStoneEnabled(cubicId: string): boolean {
  return CUBICS.find((c) => c.id === cubicId)?.stone ?? false;
}
