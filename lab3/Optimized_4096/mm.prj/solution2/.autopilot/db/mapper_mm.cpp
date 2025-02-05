#include <systemc>
#include <vector>
#include <iostream>
#include "hls_stream.h"
#include "ap_int.h"
#include "ap_fixed.h"
using namespace std;
using namespace sc_dt;
class AESL_RUNTIME_BC {
  public:
    AESL_RUNTIME_BC(const char* name) {
      file_token.open( name);
      if (!file_token.good()) {
        cout << "Failed to open tv file " << name << endl;
        exit (1);
      }
      file_token >> mName;//[[[runtime]]]
    }
    ~AESL_RUNTIME_BC() {
      file_token.close();
    }
    int read_size () {
      int size = 0;
      file_token >> mName;//[[transaction]]
      file_token >> mName;//transaction number
      file_token >> mName;//pop_size
      size = atoi(mName.c_str());
      file_token >> mName;//[[/transaction]]
      return size;
    }
  public:
    fstream file_token;
    string mName;
};
extern "C" void mm(int*, int*, int*, float, float);
extern "C" void apatb_mm_hw(volatile void * __xlx_apatb_param_C, volatile void * __xlx_apatb_param_A, volatile void * __xlx_apatb_param_B, float __xlx_apatb_param_alpha, float __xlx_apatb_param_beta) {
  // Collect __xlx_C__tmp_vec
  vector<sc_bv<32> >__xlx_C__tmp_vec;
  for (int j = 0, e = 1024; j != e; ++j) {
    __xlx_C__tmp_vec.push_back(((int*)__xlx_apatb_param_C)[j]);
  }
  int __xlx_size_param_C = 1024;
  int __xlx_offset_param_C = 0;
  int __xlx_offset_byte_param_C = 0*4;
  int* __xlx_C__input_buffer= new int[__xlx_C__tmp_vec.size()];
  for (int i = 0; i < __xlx_C__tmp_vec.size(); ++i) {
    __xlx_C__input_buffer[i] = __xlx_C__tmp_vec[i].range(31, 0).to_uint64();
  }
  // Collect __xlx_A__tmp_vec
  vector<sc_bv<32> >__xlx_A__tmp_vec;
  for (int j = 0, e = 1024; j != e; ++j) {
    __xlx_A__tmp_vec.push_back(((int*)__xlx_apatb_param_A)[j]);
  }
  int __xlx_size_param_A = 1024;
  int __xlx_offset_param_A = 0;
  int __xlx_offset_byte_param_A = 0*4;
  int* __xlx_A__input_buffer= new int[__xlx_A__tmp_vec.size()];
  for (int i = 0; i < __xlx_A__tmp_vec.size(); ++i) {
    __xlx_A__input_buffer[i] = __xlx_A__tmp_vec[i].range(31, 0).to_uint64();
  }
  // Collect __xlx_B__tmp_vec
  vector<sc_bv<32> >__xlx_B__tmp_vec;
  for (int j = 0, e = 1024; j != e; ++j) {
    __xlx_B__tmp_vec.push_back(((int*)__xlx_apatb_param_B)[j]);
  }
  int __xlx_size_param_B = 1024;
  int __xlx_offset_param_B = 0;
  int __xlx_offset_byte_param_B = 0*4;
  int* __xlx_B__input_buffer= new int[__xlx_B__tmp_vec.size()];
  for (int i = 0; i < __xlx_B__tmp_vec.size(); ++i) {
    __xlx_B__input_buffer[i] = __xlx_B__tmp_vec[i].range(31, 0).to_uint64();
  }
  // DUT call
  mm(__xlx_C__input_buffer, __xlx_A__input_buffer, __xlx_B__input_buffer, __xlx_apatb_param_alpha, __xlx_apatb_param_beta);
// print __xlx_apatb_param_C
  sc_bv<32>*__xlx_C_output_buffer = new sc_bv<32>[__xlx_size_param_C];
  for (int i = 0; i < __xlx_size_param_C; ++i) {
    __xlx_C_output_buffer[i] = __xlx_C__input_buffer[i+__xlx_offset_param_C];
  }
  for (int i = 0; i < __xlx_size_param_C; ++i) {
    ((int*)__xlx_apatb_param_C)[i] = __xlx_C_output_buffer[i].to_uint64();
  }
// print __xlx_apatb_param_A
  sc_bv<32>*__xlx_A_output_buffer = new sc_bv<32>[__xlx_size_param_A];
  for (int i = 0; i < __xlx_size_param_A; ++i) {
    __xlx_A_output_buffer[i] = __xlx_A__input_buffer[i+__xlx_offset_param_A];
  }
  for (int i = 0; i < __xlx_size_param_A; ++i) {
    ((int*)__xlx_apatb_param_A)[i] = __xlx_A_output_buffer[i].to_uint64();
  }
// print __xlx_apatb_param_B
  sc_bv<32>*__xlx_B_output_buffer = new sc_bv<32>[__xlx_size_param_B];
  for (int i = 0; i < __xlx_size_param_B; ++i) {
    __xlx_B_output_buffer[i] = __xlx_B__input_buffer[i+__xlx_offset_param_B];
  }
  for (int i = 0; i < __xlx_size_param_B; ++i) {
    ((int*)__xlx_apatb_param_B)[i] = __xlx_B_output_buffer[i].to_uint64();
  }
}
