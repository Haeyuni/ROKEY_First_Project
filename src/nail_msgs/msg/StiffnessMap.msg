std_msgs/Header header

string  session_id                    # 필수. E_MAP_SESSION_MISMATCH 검증용
string  frame_id                      # 좌표 기준 프레임

nail_msgs/StiffnessPoint[] points     # coarse + fine 통합

# --- 2단계 스캔 메타 ---
float64 coarse_pitch_mm
float64 fine_pitch_mm
int32   coarse_point_count
int32   fine_point_count
int32   candidate_count               # 경계 후보 쌍 개수

# --- 판정 결과 ---
bool    valid                         # false면 후속 공정 전부 진입 불가
float64 threshold_k_n_per_mm          # 손톱/피부 분리 임계 강성
float64 separation_margin             # 군집간거리 / 군집내분산
int32   cluster_hard_count
int32   cluster_soft_count
string  reject_reason                 # valid=false 사유

nail_msgs/BoundaryRegion region

builtin_interfaces/Time created_at
