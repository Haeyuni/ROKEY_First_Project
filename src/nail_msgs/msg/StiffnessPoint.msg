# 한 점의 압입 측정 결과. 스캔과 택프리 검사가 공유한다.

string SRC_COARSE = "coarse"
string SRC_FINE   = "fine"
string SRC_VERIFY = "verify"
string SRC_SURVEY = "survey"           # 작업면 평면 추정용 사전 측정

geometry_msgs/Point position          # nail_frame 또는 nail_local_frame 기준, mm
float64 stiffness_n_per_mm            # 대표 강성. 회귀 또는 계단 압입에서 온다
float64 release_force_n               # 이탈 시 최저 Fz. 음수면 점착
float64 hysteresis_ratio              # 로딩/언로딩 면적비. 연조직 보조 판별
float64 contact_depth_mm              # 접촉 후 압입 깊이
float64 lateral_force_n               # 접촉 시 측면 힘. 비스듬 접촉 검출
string  source                        # SRC_*
bool    valid                         # false면 미접촉 또는 측정 실패

# --- v0.3 다특징 판별 -------------------------------------------------------------
# 강성 하나로는 손톱/피부가 갈리지 않는다. 위치제어 로봇으로 단단한 면을
# 누르면 측정되는 기울기는 재질이 아니라 로봇·툴 컴플라이언스라 상한에서
# 포화하고, 단단할수록 힘 상한에 빨리 닿아 회귀할 표본조차 남지 않는다.
# 아래 축들은 전부 무차원이거나 자기 자신으로 정규화돼 있어 센서 바이어스와
# 로봇 강성에 둔감하다. 판정은 여전히 scan_node 소관이다 (NIS §5.1).

float64 incremental_stiffness_n_per_mm  # 계단 압입 ΔF/Δd. 회귀보다 강건
float64 linearity_r2                    # 하강 구간 선형 적합도
float64 power_exponent                  # f ∝ d^p 의 p. 피부 2~3, 손톱 1 근처
float64 relaxation_ratio                # 유지 중 힘 감소율. 피부(점탄성)가 크다
float64 contact_z_mm                    # 접촉 시작 높이 (base 프레임). 기하 증거
# 접근 자세에서 접촉까지 내려간 거리. contact_z_mm 과 달리 프레임 정의에
# 무관하게 "이 점은 얼마나 내려가야 닿는가"를 직접 말해주므로, 기울어진
# 작업면에서 점마다 접근 높이를 맞추는 보정의 입력이 된다.
float64 contact_travel_mm
float64 height_residual_mm              # 작업면 평면 대비 높이. scan_node가 채움
float64 surface_tilt_deg                # 접촉력 방향으로 추정한 국소 기울기
float64 slip_ratio                      # |F_접선| / |F_법선|
int32   slip_events                     # 스틱-슬립 급락 횟수
float64 nail_score                      # 융합 판별축 위 위치. scan_node가 채움

# 특징군별 신뢰 플래그. **0.0 을 측정값으로 읽지 않기 위한 관문**이다 —
# 단단한 손톱은 회귀 표본이 없어 linearity_r2 가 0.0 으로 남는데, 그걸
# "선형성이 나쁘다"로 읽으면 라벨이 통째로 뒤집힌다.
bool    stiffness_valid                 # stiffness_n_per_mm 사용 가능
bool    curve_valid                     # linearity_r2 / power_exponent 사용 가능
bool    hold_valid                      # relaxation_ratio / hysteresis_ratio 사용 가능
# NO_CONTACT          최대 깊이까지 내려가도 표면이 없었다 → 예산/안착 확인
# ALREADY_IN_CONTACT  접근 자세에서 이미 눌려 있었다 → 접근 높이를 올릴 것
# SHALLOW_INDENT      너무 단단해 회귀할 압입 구간이 없었다 (손톱에서 정상)
# SLIPPED             접선력이 마찰한계를 넘었다 → 힘 특징 폐기, 높이만 사용
string  reject_reason
