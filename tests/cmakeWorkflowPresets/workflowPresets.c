#include <jansson.h>
int main(int argc, char const *argv[])
{
    json_t *obj = json_load_file("CMakePresets.json", 0, NULL);
    if (obj == NULL)
    {
        perror("CMakePresets.json");
        return 0;
    }
    
    json_t *wlarr = NULL;
    json_unpack(obj, "{so}", "workflowPresets", &wlarr);
    // configure build test package
    size_t index;
    for (size_t i = 1; argv[i]; ++i)
    {
        printf("%s", json_dumps(wlarr, 0));
        json_t *preset = NULL;
        // json_unpack(wlarr,"")
        // system("cmake -h");
    }
    json_decref(obj);
    return 0;
}