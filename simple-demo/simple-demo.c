#include <julia.h>
JULIA_DEFINE_FAST_TLS // only define this once, in an executable (not in a shared library) if you want fast code.

#include <stdio.h> // required for `printf()`
#include <stdlib.h> // required for `rand()`

// Forward declaration
void print_arrays(double* data, double* result, int size);

int main(int argc, char *argv[]) {
  // set up the Julia context
  jl_init();

  // Create an array in C and fill it with random data
  srand(0);
  double* data = malloc(sizeof(double)*10);
  for (int i = 0; i < 10; i++) {
    data[i] = (double)rand() / (double)RAND_MAX;
  }

  // Create a second array that will hold the results, initialize to zero
  double* result = malloc(sizeof(double)*10);
  for (int i = 0; i < 10; i++) {
    result[i] = 0.0;
  }

  // Print contents
  printf("Original contents:\n");
  print_arrays(data, result, 10);
  printf("\n");

  // Load Julia file with function definition
  jl_eval_string("include(\"simple-demo.jl\")");

  // Create appropriate Julia array wrappers
  jl_value_t* array_type = jl_apply_array_type((jl_value_t*)jl_float64_type, 1);
  jl_array_t* data_jl = jl_ptr_to_array_1d(array_type, data, 10, 0);
  jl_array_t* result_jl = jl_ptr_to_array_1d(array_type, result, 10, 0);

  // Call function `double_me` from Julia
  jl_function_t* double_me_jl = jl_get_function(jl_main_module, "double_me_julia_style");
  jl_call2(double_me_jl, (jl_value_t*)result_jl, (jl_value_t*)data_jl);

  // Print contents again
  printf("Contents after call to `double_me_julia_style`:\n");
  print_arrays(data, result, 10);

  // This is an alternative approach, where the conversion between Julia and C is handled on the
  // Julia side

  // Reset results
  for (int i = 0; i < 10; i++) {
    result[i] = 0.0;
  }
  printf("\nContents after reset:\n");
  print_arrays(data, result, 10);
  printf("\n");
  // Get function pointer to the Julia function
  void (*double_me_c_style)(double*, double*) = jl_unbox_voidpointer(
      jl_eval_string("@cfunction(double_me_c_style, Cvoid, (Ptr{Cdouble}, Ptr{Cdouble}))"));

  // Call function `double_me2` defined in Julia
  double_me_c_style(result, data);

  // Print contents again
  printf("Contents after call to `double_me_c_style`:\n");
  print_arrays(data, result, 10);

  // perform clean-up tasks in Julia
  jl_atexit_hook(0);

  // Free memory
  free(data);
  free(result);
  return 0;
}

void print_arrays(double* data, double* result, int size) {
  printf("data:  ");
  for (int i = 0; i < size; i++) {
    printf(" %5.3f", data[i]);
  }
  printf("\n");
  printf("result:");
  for (int i = 0; i < size; i++) {
    printf(" %5.3f", result[i]);
  }
  printf("\n");
}
