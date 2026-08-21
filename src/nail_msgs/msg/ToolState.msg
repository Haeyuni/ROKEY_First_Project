std_msgs/Header header

string NONE     = "none"
string PROBE    = "probe"
string SANDER   = "sander"
string BRUSH    = "brush"
string COATER   = "coater"
string UV       = "uv"
string TWEEZERS = "tweezers"

string  current_tool
string  active_tcp                    # tcp_probe, tcp_sander, …
float64 grip_width_mm
float64 expected_width_mm
bool    grip_verified
