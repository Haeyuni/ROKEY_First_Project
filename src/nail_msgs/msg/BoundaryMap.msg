std_msgs/Header header
string session_id

nail_msgs/ProbeMeasurement[] measurements
geometry_msgs/Point[] boundary_polygon

float64 coarse_pitch_mm
float64 fine_pitch_mm
int32 coarse_point_count
int32 fine_point_count
int32 boundary_candidate_count
float64 contact_ratio
bool valid
string reject_reason
