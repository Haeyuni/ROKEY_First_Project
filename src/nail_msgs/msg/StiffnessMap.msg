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

# --- v0.3 다특징 판별 메타 ---------------------------------------------------------
string  classifier_method             # "fused" | "stiffness"
float64 discriminant_threshold        # 융합 판별축 상의 임계값
string[] feature_names                # 융합에 실제로 쓰인 축
float64[] feature_margins             # 축별 1차원 분리도 (튜닝의 근거)
float64[] feature_weights             # 축별 가중치
float64 height_step_mm                # 손톱/피부 평균 높이차
# 단차 / 배경 잔차. 재질이 같아 힘 축이 죽은 경우 이 값이 경계 신뢰도다 —
# separation_margin 은 강성 군집화용 척도라 기하 축에는 맞지 않는다.
float64 height_step_snr
int32   spatial_flips                 # 공간 일관성 보정으로 뒤집힌 점 수
float64 work_plane_tilt_deg           # 사전 측정한 작업면 기울기
float64 work_plane_rms_mm             # 평면 적합 잔차. 크면 평면 가정 실패

nail_msgs/BoundaryRegion region

builtin_interfaces/Time created_at
