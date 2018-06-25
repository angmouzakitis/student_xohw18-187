/* ---------------------------------------------------------
 * Functions : roupNameFromId, groupIdFromName, grouplist 
 *
* ------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>
#include <ctype.h>
#include <sys/types.h> 
#include <unistd.h>   
#include <limits.h>

 /* Function that returns group id corresponding to group name */
char *groupNameFromId(gid_t gid)
{
    struct group *grp;

    grp = getgrgid(gid);
    return (grp == NULL) ? NULL : grp->gr_name;
}

 /* Function thas returns group name corresponding to group id */
gid_t groupIdFromName(const char *name)
{
    struct group *grp;
    gid_t g;
    char *endptr;

    if (name == NULL || *name == '\0')  /* On NULL or empty string */
        return -1;                      /* return an error */

    g = strtol(name, &endptr, 10);      /* As a convenience to caller */
    if (*endptr == '\0')                /* allow a numeric string */
        return g;

    grp = getgrnam(name);
    if (grp == NULL)
        return -1;

    return grp->gr_gid;
}

/* Function that returns grouplist */
int grouplist(gid_t *groups, int *ngroups)
{
   register struct passwd *pw;
   register uid_t uid;

   uid = geteuid();	//get effective user
   pw = getpwuid(uid);	//get password entry of efective user
   if(pw){   

   	if ( getgrouplist( pw->pw_name, pw->pw_gid, groups, ngroups) == -1) {
		   printf ("Groups array is too small: %d\n", *ngroups);
   	}

   	return EXIT_SUCCESS;

   }else{

	printf("Cannot find username for UID:%u\n",(unsigned)uid);
	return EXIT_FAILURE;

   }
}

