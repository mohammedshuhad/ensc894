#include <systemc>
#include <iostream>
#include <cstdlib>
#include <cstddef>
#include <stdint.h>
#include "SysCFileHandler.h"
#include "ap_int.h"
#include "ap_fixed.h"
#include <complex>
#include <stdbool.h>
#include "autopilot_cbe.h"
#include "hls_stream.h"
#include "hls_half.h"
#include "hls_signal_handler.h"

using namespace std;
using namespace sc_core;
using namespace sc_dt;

// wrapc file define:
#define AUTOTB_TVIN_C "../tv/cdatafile/c.mm.autotvin_C.dat"
#define AUTOTB_TVOUT_C "../tv/cdatafile/c.mm.autotvout_C.dat"
// wrapc file define:
#define AUTOTB_TVIN_A "../tv/cdatafile/c.mm.autotvin_A.dat"
#define AUTOTB_TVOUT_A "../tv/cdatafile/c.mm.autotvout_A.dat"
// wrapc file define:
#define AUTOTB_TVIN_B "../tv/cdatafile/c.mm.autotvin_B.dat"
#define AUTOTB_TVOUT_B "../tv/cdatafile/c.mm.autotvout_B.dat"
// wrapc file define:
#define AUTOTB_TVIN_alpha "../tv/cdatafile/c.mm.autotvin_alpha.dat"
#define AUTOTB_TVOUT_alpha "../tv/cdatafile/c.mm.autotvout_alpha.dat"
// wrapc file define:
#define AUTOTB_TVIN_beta "../tv/cdatafile/c.mm.autotvin_beta.dat"
#define AUTOTB_TVOUT_beta "../tv/cdatafile/c.mm.autotvout_beta.dat"

#define INTER_TCL "../tv/cdatafile/ref.tcl"

// tvout file define:
#define AUTOTB_TVOUT_PC_C "../tv/rtldatafile/rtl.mm.autotvout_C.dat"
// tvout file define:
#define AUTOTB_TVOUT_PC_A "../tv/rtldatafile/rtl.mm.autotvout_A.dat"
// tvout file define:
#define AUTOTB_TVOUT_PC_B "../tv/rtldatafile/rtl.mm.autotvout_B.dat"
// tvout file define:
#define AUTOTB_TVOUT_PC_alpha "../tv/rtldatafile/rtl.mm.autotvout_alpha.dat"
// tvout file define:
#define AUTOTB_TVOUT_PC_beta "../tv/rtldatafile/rtl.mm.autotvout_beta.dat"

class INTER_TCL_FILE {
  public:
INTER_TCL_FILE(const char* name) {
  mName = name; 
  C_depth = 0;
  A_depth = 0;
  B_depth = 0;
  alpha_depth = 0;
  beta_depth = 0;
  trans_num =0;
}
~INTER_TCL_FILE() {
  mFile.open(mName);
  if (!mFile.good()) {
    cout << "Failed to open file ref.tcl" << endl;
    exit (1); 
  }
  string total_list = get_depth_list();
  mFile << "set depth_list {\n";
  mFile << total_list;
  mFile << "}\n";
  mFile << "set trans_num "<<trans_num<<endl;
  mFile.close();
}
string get_depth_list () {
  stringstream total_list;
  total_list << "{C " << C_depth << "}\n";
  total_list << "{A " << A_depth << "}\n";
  total_list << "{B " << B_depth << "}\n";
  total_list << "{alpha " << alpha_depth << "}\n";
  total_list << "{beta " << beta_depth << "}\n";
  return total_list.str();
}
void set_num (int num , int* class_num) {
  (*class_num) = (*class_num) > num ? (*class_num) : num;
}
void set_string(std::string list, std::string* class_list) {
  (*class_list) = list;
}
  public:
    int C_depth;
    int A_depth;
    int B_depth;
    int alpha_depth;
    int beta_depth;
    int trans_num;
  private:
    ofstream mFile;
    const char* mName;
};

static void RTLOutputCheckAndReplacement(std::string &AESL_token, std::string PortName) {
  bool no_x = false;
  bool err = false;

  no_x = false;
  // search and replace 'X' with '0' from the 3rd char of token
  while (!no_x) {
    size_t x_found = AESL_token.find('X', 0);
    if (x_found != string::npos) {
      if (!err) { 
        cerr << "WARNING: [SIM 212-201] RTL produces unknown value 'X' on port" 
             << PortName << ", possible cause: There are uninitialized variables in the C design."
             << endl; 
        err = true;
      }
      AESL_token.replace(x_found, 1, "0");
    } else
      no_x = true;
  }
  no_x = false;
  // search and replace 'x' with '0' from the 3rd char of token
  while (!no_x) {
    size_t x_found = AESL_token.find('x', 2);
    if (x_found != string::npos) {
      if (!err) { 
        cerr << "WARNING: [SIM 212-201] RTL produces unknown value 'x' on port" 
             << PortName << ", possible cause: There are uninitialized variables in the C design."
             << endl; 
        err = true;
      }
      AESL_token.replace(x_found, 1, "0");
    } else
      no_x = true;
  }
}
extern "C" void mm_hw_stub_wrapper(volatile void *, volatile void *, volatile void *, float, float);

extern "C" void apatb_mm_hw(volatile void * __xlx_apatb_param_C, volatile void * __xlx_apatb_param_A, volatile void * __xlx_apatb_param_B, float __xlx_apatb_param_alpha, float __xlx_apatb_param_beta) {
  refine_signal_handler();
  fstream wrapc_switch_file_token;
  wrapc_switch_file_token.open(".hls_cosim_wrapc_switch.log");
  int AESL_i;
  if (wrapc_switch_file_token.good())
  {

    CodeState = ENTER_WRAPC_PC;
    static unsigned AESL_transaction_pc = 0;
    string AESL_token;
    string AESL_num;{
      static ifstream rtl_tv_out_file;
      if (!rtl_tv_out_file.is_open()) {
        rtl_tv_out_file.open(AUTOTB_TVOUT_PC_C);
        if (rtl_tv_out_file.good()) {
          rtl_tv_out_file >> AESL_token;
          if (AESL_token != "[[[runtime]]]")
            exit(1);
        }
      }
  
      if (rtl_tv_out_file.good()) {
        rtl_tv_out_file >> AESL_token; 
        rtl_tv_out_file >> AESL_num;  // transaction number
        if (AESL_token != "[[transaction]]") {
          cerr << "Unexpected token: " << AESL_token << endl;
          exit(1);
        }
        if (atoi(AESL_num.c_str()) == AESL_transaction_pc) {
          std::vector<sc_bv<32> > C_pc_buffer(16777216);
          int i = 0;

          rtl_tv_out_file >> AESL_token; //data
          while (AESL_token != "[[/transaction]]"){

            RTLOutputCheckAndReplacement(AESL_token, "C");
  
            // push token into output port buffer
            if (AESL_token != "") {
              C_pc_buffer[i] = AESL_token.c_str();;
              i++;
            }
  
            rtl_tv_out_file >> AESL_token; //data or [[/transaction]]
            if (AESL_token == "[[[/runtime]]]" || rtl_tv_out_file.eof())
              exit(1);
          }
          if (i > 0) {{
            int i = 0;
            for (int j = 0, e = 16777216; j < e; j += 1, ++i) {
            ((int*)__xlx_apatb_param_C)[j] = C_pc_buffer[i].to_int64();
          }}}
        } // end transaction
      } // end file is good
    } // end post check logic bolck
  
    AESL_transaction_pc++;
    return ;
  }
static unsigned AESL_transaction;
static AESL_FILE_HANDLER aesl_fh;
static INTER_TCL_FILE tcl_file(INTER_TCL);
std::vector<char> __xlx_sprintf_buffer(1024);
CodeState = ENTER_WRAPC;
//C
aesl_fh.touch(AUTOTB_TVIN_C);
aesl_fh.touch(AUTOTB_TVOUT_C);
//A
aesl_fh.touch(AUTOTB_TVIN_A);
aesl_fh.touch(AUTOTB_TVOUT_A);
//B
aesl_fh.touch(AUTOTB_TVIN_B);
aesl_fh.touch(AUTOTB_TVOUT_B);
//alpha
aesl_fh.touch(AUTOTB_TVIN_alpha);
aesl_fh.touch(AUTOTB_TVOUT_alpha);
//beta
aesl_fh.touch(AUTOTB_TVIN_beta);
aesl_fh.touch(AUTOTB_TVOUT_beta);
CodeState = DUMP_INPUTS;
unsigned __xlx_offset_byte_param_C = 0;
// print C Transactions
{
  sprintf(__xlx_sprintf_buffer.data(), "[[transaction]] %d\n", AESL_transaction);
  aesl_fh.write(AUTOTB_TVIN_C, __xlx_sprintf_buffer.data());
  {  __xlx_offset_byte_param_C = 0*4;
  if (__xlx_apatb_param_C) {
    for (int j = 0  - 0, e = 16777216 - 0; j != e; ++j) {
sc_bv<32> __xlx_tmp_lv = ((int*)__xlx_apatb_param_C)[j];

    sprintf(__xlx_sprintf_buffer.data(), "%s\n", __xlx_tmp_lv.to_string(SC_HEX).c_str());
    aesl_fh.write(AUTOTB_TVIN_C, __xlx_sprintf_buffer.data()); 
      }
  }
}
  tcl_file.set_num(16777216, &tcl_file.C_depth);
  sprintf(__xlx_sprintf_buffer.data(), "[[/transaction]] \n");
  aesl_fh.write(AUTOTB_TVIN_C, __xlx_sprintf_buffer.data());
}
unsigned __xlx_offset_byte_param_A = 0;
// print A Transactions
{
  sprintf(__xlx_sprintf_buffer.data(), "[[transaction]] %d\n", AESL_transaction);
  aesl_fh.write(AUTOTB_TVIN_A, __xlx_sprintf_buffer.data());
  {  __xlx_offset_byte_param_A = 0*4;
  if (__xlx_apatb_param_A) {
    for (int j = 0  - 0, e = 16777216 - 0; j != e; ++j) {
sc_bv<32> __xlx_tmp_lv = ((int*)__xlx_apatb_param_A)[j];

    sprintf(__xlx_sprintf_buffer.data(), "%s\n", __xlx_tmp_lv.to_string(SC_HEX).c_str());
    aesl_fh.write(AUTOTB_TVIN_A, __xlx_sprintf_buffer.data()); 
      }
  }
}
  tcl_file.set_num(16777216, &tcl_file.A_depth);
  sprintf(__xlx_sprintf_buffer.data(), "[[/transaction]] \n");
  aesl_fh.write(AUTOTB_TVIN_A, __xlx_sprintf_buffer.data());
}
unsigned __xlx_offset_byte_param_B = 0;
// print B Transactions
{
  sprintf(__xlx_sprintf_buffer.data(), "[[transaction]] %d\n", AESL_transaction);
  aesl_fh.write(AUTOTB_TVIN_B, __xlx_sprintf_buffer.data());
  {  __xlx_offset_byte_param_B = 0*4;
  if (__xlx_apatb_param_B) {
    for (int j = 0  - 0, e = 16777216 - 0; j != e; ++j) {
sc_bv<32> __xlx_tmp_lv = ((int*)__xlx_apatb_param_B)[j];

    sprintf(__xlx_sprintf_buffer.data(), "%s\n", __xlx_tmp_lv.to_string(SC_HEX).c_str());
    aesl_fh.write(AUTOTB_TVIN_B, __xlx_sprintf_buffer.data()); 
      }
  }
}
  tcl_file.set_num(16777216, &tcl_file.B_depth);
  sprintf(__xlx_sprintf_buffer.data(), "[[/transaction]] \n");
  aesl_fh.write(AUTOTB_TVIN_B, __xlx_sprintf_buffer.data());
}
// print alpha Transactions
{
  sprintf(__xlx_sprintf_buffer.data(), "[[transaction]] %d\n", AESL_transaction);
  aesl_fh.write(AUTOTB_TVIN_alpha, __xlx_sprintf_buffer.data());
  {
    sc_bv<32> __xlx_tmp_lv = *((int*)&__xlx_apatb_param_alpha);

    sprintf(__xlx_sprintf_buffer.data(), "%s\n", __xlx_tmp_lv.to_string(SC_HEX).c_str());
    aesl_fh.write(AUTOTB_TVIN_alpha, __xlx_sprintf_buffer.data()); 
  }
  tcl_file.set_num(1, &tcl_file.alpha_depth);
  sprintf(__xlx_sprintf_buffer.data(), "[[/transaction]] \n");
  aesl_fh.write(AUTOTB_TVIN_alpha, __xlx_sprintf_buffer.data());
}
// print beta Transactions
{
  sprintf(__xlx_sprintf_buffer.data(), "[[transaction]] %d\n", AESL_transaction);
  aesl_fh.write(AUTOTB_TVIN_beta, __xlx_sprintf_buffer.data());
  {
    sc_bv<32> __xlx_tmp_lv = *((int*)&__xlx_apatb_param_beta);

    sprintf(__xlx_sprintf_buffer.data(), "%s\n", __xlx_tmp_lv.to_string(SC_HEX).c_str());
    aesl_fh.write(AUTOTB_TVIN_beta, __xlx_sprintf_buffer.data()); 
  }
  tcl_file.set_num(1, &tcl_file.beta_depth);
  sprintf(__xlx_sprintf_buffer.data(), "[[/transaction]] \n");
  aesl_fh.write(AUTOTB_TVIN_beta, __xlx_sprintf_buffer.data());
}
CodeState = CALL_C_DUT;
mm_hw_stub_wrapper(__xlx_apatb_param_C, __xlx_apatb_param_A, __xlx_apatb_param_B, __xlx_apatb_param_alpha, __xlx_apatb_param_beta);
CodeState = DUMP_OUTPUTS;
// print C Transactions
{
  sprintf(__xlx_sprintf_buffer.data(), "[[transaction]] %d\n", AESL_transaction);
  aesl_fh.write(AUTOTB_TVOUT_C, __xlx_sprintf_buffer.data());
  {  __xlx_offset_byte_param_C = 0*4;
  if (__xlx_apatb_param_C) {
    for (int j = 0  - 0, e = 16777216 - 0; j != e; ++j) {
sc_bv<32> __xlx_tmp_lv = ((int*)__xlx_apatb_param_C)[j];

    sprintf(__xlx_sprintf_buffer.data(), "%s\n", __xlx_tmp_lv.to_string(SC_HEX).c_str());
    aesl_fh.write(AUTOTB_TVOUT_C, __xlx_sprintf_buffer.data()); 
      }
  }
}
  tcl_file.set_num(16777216, &tcl_file.C_depth);
  sprintf(__xlx_sprintf_buffer.data(), "[[/transaction]] \n");
  aesl_fh.write(AUTOTB_TVOUT_C, __xlx_sprintf_buffer.data());
}
CodeState = DELETE_CHAR_BUFFERS;
AESL_transaction++;
tcl_file.set_num(AESL_transaction , &tcl_file.trans_num);
}
