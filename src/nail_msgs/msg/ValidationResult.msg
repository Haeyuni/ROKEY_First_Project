string POINT_CENTER = "center"
string POINT_LEFT   = "left"
string POINT_RIGHT  = "right"

string RESULT_PASS  = "PASS"
string RESULT_FAIL  = "FAIL"
string RESULT_SKIP  = "SKIP"     # 마진 부족으로 측정 안 함

string  session_id
int32   layer_index
string  point_label              # POINT_*
geometry_msgs/Point position

float64 release_force_n
float64 stiffness_n_per_mm
float64 threshold_n              # 판정에 쓴 임계값 (사후 재해석용)
string  result

nail_msgs/ForceSample[] waveform # FAIL 시 필수. PASS 시 생략 가능
builtin_interfaces/Time measured_at
