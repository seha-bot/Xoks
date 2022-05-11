typedef struct {
    unsigned int size;
} nec_array;

#define nec(a) ((nec_array*)(a) - 1)
#define nec_size(a) ((a) ? nec(a)->size : 0)
#define nec_realloc(a, s)\
({\
    void* __nec_b;\
    int __nec_s = (s);\
    (a) ?\
    ((a) = realloc(nec(a), __nec_s * sizeof(*(a)) + sizeof(nec_array)), __nec_b = (nec_array*)(a) + 1, (a) = __nec_b):\
    ((a) = realloc((a), __nec_s * sizeof(*(a)) + sizeof(nec_array)), __nec_b = (nec_array*)(a) + 1, (a) = __nec_b);\
    nec(a)->size = __nec_s;\
})
// #define nec_realloc_typecast(t, a, s) a = realloc(a, s * sizeof(t) + sizeof(nec_array))
#define nec_free(a) ({ void* __nec_b = nec(a); free(__nec_b); a = 0; })
#define nec_push(ar, v)\
({\
    nec_realloc(ar, nec_size(ar) + 1);\
    (ar)[nec_size(ar) - 1] = (v);\
})

// #define nec_push_typecast(t, a, v)\
// {\
//     void* __nec_b; t* c; (a) ?\
//     (__nec_b = (nec_array*)(a) - 1, a = __nec_b, nec_realloc_typecast(t, a, ++((nec_array*)(a))->size), __nec_b = (nec_array*)(a) + 1, (a) = __nec_b, 0):\
//     ((nec_realloc_typecast(t, a, 1), __nec_b = (nec_array*)(a) + 1, nec(__nec_b)->size = 1, (a) = __nec_b), 0),\
//     c = (t*)(a),\
//     c[nec_size(a) - 1] = v,\
//     a = (void*)c;\
// }

#define nec_remove_at(t, a, i)\
({\
    if(i >= 0 && i < nec_size(a))\
    {\
        if(nec_size(a) == 1)\
        {\
            nec_free(a);\
            (a) = 0;\
        }\
        else if(i == nec_size(a) - 1) nec_realloc(a, nec_size(a) - 1);\
        else\
        {\
            void* ___nec_b = (a);\
            (a) = 0;\
            nec_realloc(a, nec_size(___nec_b) - 1);\
            int off = 0;\
            for(int it = 0; it < nec_size(___nec_b); it++)\
            {\
                if(it != i) (a)[it - off] = ((t*)(___nec_b))[it];\
                else off++;\
            }\
            nec_free(___nec_b);\
        }\
    }\
})

#define nec_remove(t, a, v)\
({\
    for(int i = 0; i < nec_size(a); i++)\
    {\
        if((a)[i] == v)\
        {\
            nec_remove_at(t, a, i);\
            break;\
        }\
    }\
})


/*
TODO:
add nec_push_norealloc
fix nec_push_typecast, nec_realloc_typecast
*/