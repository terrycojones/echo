/* $Header: /tmp_mnt/vida/disks/disk5/Users/terry/r/echo/RCS/resources.h,v 1.4 1992/10/22 04:14:17 terry Exp $ */
extern void add_resources();
extern void sub_resources();
extern void zero_resources();
extern void set_resources();
extern RESOURCE_SIZE total_resources();
extern void print_resource_list();
extern void print_terse_resource_list();

#define IS_VALID_RESOURCE(x) ((x) >= 'a' && (x) < 'a' + nresources)
