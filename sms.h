#define MAX_P_NUM_LEN 15
#define MAX_MSG_LEN 140

typedef struct Message_t {
  char remoteNumber[MAX_P_NUM_LEN];
  char message[MAX_MSG_LEN];
} Message_t;
