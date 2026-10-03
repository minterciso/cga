#include "params.h"

#include <stdlib.h>
#include <getopt.h>
#include "consts.h"

Params params = { DEFAULT_MUT_RATE, DEFAULT_CROSS_RATE, DEFAULT_MODE };

static void usage(FILE *stream, const char *prog)
{
  fprintf(stream,
          "Usage: %s [options]\n"
          "  -m, --mutation-rate R   per-bit mutation probability in [0,1] (default %g)\n"
          "  -c, --crossover-rate R  per-position chance (%%) of stopping the crossover\n"
          "                          point search, in [0,100] (default %g). This is NOT\n"
          "                          the p_c crossover probability of MCH/CMD.\n"
          "  -M, --mode N            rule symbols: 2 = binary, 3 = ternary (default %d)\n"
          "  -h, --help              show this help\n",
          prog, DEFAULT_MUT_RATE, DEFAULT_CROSS_RATE, DEFAULT_MODE);
}

static int parseDouble(const char *s, double min, double max, double *out)
{
  char *end = NULL;
  double v = strtod(s, &end);
  if(end == s || *end != '\0' || v < min || v > max)
    return -1;
  *out = v;
  return 0;
}

int parseParams(int argc, char *argv[])
{
  static const struct option opts[] =
  {
    {"mutation-rate",  required_argument, NULL, 'm'},
    {"crossover-rate", required_argument, NULL, 'c'},
    {"mode",           required_argument, NULL, 'M'},
    {"help",           no_argument,       NULL, 'h'},
    {NULL, 0, NULL, 0}
  };
  int opt;
  double v;

  while((opt = getopt_long(argc, argv, "m:c:M:h", opts, NULL)) != -1)
  {
    switch(opt)
    {
      case 'm':
        if(parseDouble(optarg, 0.0, 1.0, &params.mut_rate) != 0)
        {
          fprintf(stderr, "Invalid mutation rate '%s': expected a number in [0,1]\n", optarg);
          return -1;
        }
        break;
      case 'c':
        if(parseDouble(optarg, 0.0, 100.0, &params.cross_rate) != 0)
        {
          fprintf(stderr, "Invalid crossover rate '%s': expected a number in [0,100]\n", optarg);
          return -1;
        }
        break;
      case 'M':
        if(parseDouble(optarg, 2.0, 3.0, &v) != 0 || (v != 2.0 && v != 3.0))
        {
          fprintf(stderr, "Invalid mode '%s': expected 2 or 3\n", optarg);
          return -1;
        }
        params.mode = (int)v;
        break;
      case 'h':
        usage(stdout, argv[0]);
        return 1;
      default:
        usage(stderr, argv[0]);
        return -1;
    }
  }
  if(optind < argc)
  {
    fprintf(stderr, "Unexpected argument '%s'\n", argv[optind]);
    usage(stderr, argv[0]);
    return -1;
  }
  return 0;
}

void printParams(FILE *stream)
{
  fprintf(stream, "mutation-rate=%g crossover-rate=%g mode=%d\n",
          params.mut_rate, params.cross_rate, params.mode);
}
