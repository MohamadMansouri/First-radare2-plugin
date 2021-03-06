#ifndef __UTILS__
#define __UTILS__



#include <stdbool.h>
#include <r_core.h>
#include <r_socket.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>


typedef enum action {
f_test,
f_checkin,
f_add,
f_history,
f_applied,
f_unapplied,
f_delete,
f_created,
f_get,
f_scan
} action;


typedef struct binary_info{
  char* f_md5;
  char* f_sha1;
  char* f_sha256;
  int f_crc32;

}binary_info;


typedef struct Config
{
	char* _server;
	uint _port;	
	char* _protocol;
    bool _verify;
    bool _auth;
    char* _api_key;
} FS_config;



typedef struct Metadata
{
	char* id; // The First ID assosciated with the function
	char* creator; //The handle of the annotation creator
	int address; //The virtual address associated with the function
	char* name; //The name of the function // String (max_length = 128)
	char* original_name; //The orginal name of the function
	char* prototype; //The prototype of the function // String (max_length = 256)
	char* comment; //The repeatable comment associated with the function // String (max_length = 512)
	int segment; //The start address of the function's segment
	int offset; //The function offset from the start of the segment
	bool has_changed; //True if function metadata has changed
	bool is_lib; //True if function is a library function
	bool created; //True if the annotations were created by user
	char* signature; //The opcodes associated with the function // String (base64 encoded)
	int apis_size;
	char** apis; //The APIs called by the function // List of Strings (max_string_length = 64)
	// not used
}Metadata;

typedef struct MetadataServer
{
	int address; //The virtual address associated with the function
	char* name; //The name of the function
	char* prototype; //The prototype of the function
	char* comment; //The comment associated with the function
	char* creator; //The handle of the annotation creator
	char* id; //The FIRST ID associated with this metadata
	int rank; //The number of unqiue applies of this metadata
	float similarity; //The percentage of similarity between this function and the original queried for function. This value can be very rough estimate depending on the engine.
	char* engines;
	// not used yet
	
}MetadataServer;

typedef struct RespCreated
{
	int pages;
	MetadataServer* metadata;
	int size;

	
}RespCreated;


typedef struct DBdata
{
	char id[26];
	int address;
	bool deleted;
}DBdata;



// communication with server
static bool send_g(action act, char* token, char* parms, size_t callback(void *ptr, size_t size, size_t nmemb, void *stream));
static bool send_p(action act, char* token, char* parms, size_t callback(void *ptr, size_t size, size_t nmemb, void *stream));
bool s_test_connection();
static void s_check_in(action act);
static bool s_add(Metadata metadata[], int size, char* arch);	
static bool s_scan(Metadata metadata[], int size, char* arch );
static bool s_history(const char** metadata_id, int size);
static void s_get(char** metadata_id, int* address, int size, MetadataServer* m);
static void s_applied(const char* metadata_id);
static bool s_unapplied(char* metadata_id);
static bool s_delete(const char* metadata_id);








// setter and getters
bool f_set_config();
void set_hashes(RCore *core);
static char* get_token();
static char* get_arch(RCore* core);
static char* get_signature(RCore* core, const RAnalFunction* fcn);
static char** get_apis(RCore* core, RAnalFunction* fcn, int* size);
static char* get_prototype(RCore *core , RAnalFunction *fcn);
static char* get_comment(RCore* core, RAnalFunction *fcn);
static bool set_comment(RCore* core, RAnalFunction *fcn, const char* comment);






//doers
bool do_add(RCore *core,RAnalFunction *fcn);
bool do_scan(RCore *core,RAnalFunction *fcn);
bool do_add_all(RCore* core, RList* fcns, const char* comm);
bool do_scan_all(RCore* core, RList* fcns);
void do_get();
void do_delete(RCore* core, const int addr);
void do_delete_id(const char* id);
void do_history(const int addr);
void do_history_id(const char* id);
void do_created();
void do_apply(RCore* core, const char* id, int addr);






//DB
static bool save(DBdata d);
static int exist_in_file(FILE* f, DBdata d);
static void read_db();
static char* delete_db(const int addr);
static bool delete_db_unknown_file(const char id[]);
static char* check_db(const int addr);
static bool check_db_unknown_file(const char id[]);
#endif