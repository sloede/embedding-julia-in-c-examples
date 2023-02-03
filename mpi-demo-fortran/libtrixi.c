#include <julia.h>
JULIA_DEFINE_FAST_TLS // only define this once, in an executable (not in a shared library) if you want fast code.

#include <stdio.h> // required for `printf()`

#include <mpi.h> // required for MPI

#include "libtrixi.h"

// Auxiliary functions and macros
static jl_value_t* checked_eval_string(const char* code, const char* func, const char* file, int lineno);
#define LOC __func__, __FILE__, __LINE__

// Local data storage
int (*parallel_sum)(int*, int, MPI_Comm) = NULL;

void trixi_init(/* int argc, char *argv[] */) {
  // Init Julia
  jl_init();

  // TODO: This does not work yet - the `--project=.` string is not recognized by Julia :-/
  // Parse command line args
  /* char arg0[] = "mpi-demo-fortran"; */
  /* char arg1[] = "--project=."; */
  /* char* args[] = { &arg0[0], &arg1[0], NULL }; */
  /* char** argv = &args[0]; */
  /* int argc = 2; */
  /* jl_parse_opts(&argc, &argv); */

  // Activate current project
  checked_eval_string("using Pkg; Pkg.activate(\".\", io=devnull)", LOC);

  // Load Julia file with function definition
  checked_eval_string("include(\"mpi-demo.jl\")", LOC);

  // Store function pointer
  parallel_sum = jl_unbox_voidpointer(checked_eval_string("parallel_sum_cfunction()", LOC));
}

void trixi_finalize() {
  jl_atexit_hook(0);
}

int trixi_parallel_sum(int* data, int size, MPI_Fint* comm) {
  // Convert Fortran communicator type to C
  MPI_Comm comm_c = MPI_Comm_f2c(*comm);

  // Compute parallel sum
  int sum = parallel_sum(data, size, comm_c);

  return sum;
}

// Run Julia command and check for errors
//
// Adapted from the Julia repository.
// Source: https://github.com/JuliaLang/julia/blob/c0dd6ff8363f948237304821941b06d67014fa6a/test/embedding/embedding.c#L17-L31
jl_value_t* checked_eval_string(const char* code, const char* func, const char* file, int lineno) {
  jl_value_t *result = jl_eval_string(code);
  if (jl_exception_occurred()) {
      // none of these allocate, so a gc-root (JL_GC_PUSH) is not necessary
      jl_printf(jl_stderr_stream(), "ERROR in %s:%d (%s):\n", file, lineno, func);
      jl_printf(jl_stderr_stream(), "The following Julia code could not be evaluated: %s\n", code);
      jl_call2(jl_get_function(jl_base_module, "showerror"),
                jl_stderr_obj(),
                jl_exception_occurred());
      jl_printf(jl_stderr_stream(), "\n");
      jl_atexit_hook(1);
      exit(1);
  }

  assert(result && "Missing return value but no exception occurred!");

  return result;
}
