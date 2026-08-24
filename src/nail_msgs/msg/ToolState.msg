std_msgs/Header header

string NONE     = "none"
string SANDER   = "sander"
string BRUSH    = "brush"
string COATER   = "coater"
string UV       = "uv"
string TWEEZERS = "tweezers"

string  current_tool
string  active_tcp                    # tcp_sander, tcp_coater, …
float64 grip_width_mm
float64 expected_width_mm
bool    grip_verified
