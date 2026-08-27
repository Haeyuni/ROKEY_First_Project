std_msgs/Header header

string STAGE_IDLE     = "IDLE"
string STAGE_PRECHECK = "PRECHECK"
string STAGE_SAND     = "SAND"
string STAGE_BRUSH    = "BRUSH"
string STAGE_COAT     = "COAT"
string STAGE_CURE     = "CURE"
string STAGE_STONE    = "STONE"
string STAGE_TOOL_CHANGE = "TOOL_CHANGE"
string STAGE_FINISH   = "FINISH"
string STAGE_ABORTED  = "ABORTED"

string  session_id
string  stage
int32   layer_index
int32   layer_total
float64 stage_percent
float64 session_percent
string  current_tool
nail_msgs/ErrorCode last_error
