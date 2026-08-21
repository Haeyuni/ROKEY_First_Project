geometry_msgs/Point[] boundary_polygon    # 고강성(가공 허용) 외곽선
geometry_msgs/Point[] forbidden_polygon   # 저강성(가공 금지)
geometry_msgs/Point[] coat_polygon        # boundary에서 offset 안쪽

float64 boundary_offset_mm
float64 repeat_deviation_mm               # 반복 측정 편차
bool    reliable
