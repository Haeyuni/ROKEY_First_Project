# 한 점의 압입 측정 결과. 스캔과 택프리 검사가 공유한다.

string SRC_COARSE = "coarse"
string SRC_FINE   = "fine"
string SRC_VERIFY = "verify"

geometry_msgs/Point position          # nail_frame 또는 nail_local_frame 기준, mm
float64 stiffness_n_per_mm            # 하강 구간 선형회귀 기울기
float64 release_force_n               # 이탈 시 최저 Fz. 음수면 점착
float64 hysteresis_ratio              # 로딩/언로딩 면적비. 연조직 보조 판별
float64 contact_depth_mm              # 접촉 후 압입 깊이
float64 lateral_force_n               # 접촉 시 측면 힘. 비스듬 접촉 검출
string  source                        # SRC_*
bool    valid                         # false면 미접촉 또는 측정 실패
