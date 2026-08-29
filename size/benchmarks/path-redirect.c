extern int size_compare(const char *, const char *);
extern char *size_copy(char *, const char *);
extern char *size_find_last(const char *, int);
extern int size_open(char *, unsigned int, unsigned int, unsigned int *);

int size_path_redirect(char *path, unsigned int flags, unsigned int mode,
                       unsigned int *error) {
  static const char source_name[] = "source.bin";
  static const char target_name[] = "target.bin";
  char redirected[128];

  if (path != 0) {
    char *name = size_find_last(path, '\\');
    if (name != 0)
      ++name;
    else
      name = path;

    if (size_compare(name, source_name) == 0) {
      unsigned int prefix = name - path;
      if (prefix + sizeof(target_name) <= sizeof(redirected)) {
        size_copy(redirected, path);
        size_copy(redirected + prefix, target_name);
        path = redirected;
      }
    }
  }

  return size_open(path, flags, mode, error);
}
