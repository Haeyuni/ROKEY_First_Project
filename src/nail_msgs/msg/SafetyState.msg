std_msgs/Header header

bool safe_to_move                     # 전 노드가 이 값 하나로 판단

bool estop_released
bool comm_ok
bool tool_grip_ok

string FAULT_ESTOP        = "FAULT_ESTOP"
string FAULT_COMM_LOST    = "FAULT_COMM_LOST"
string FAULT_TOOL_DROP    = "FAULT_TOOL_DROP"

string[] active_faults                # 비어 있어야 safe_to_move 가능
string   reason                       # 사람이 읽을 설명
