/*************************************************************************
	> File Name: easyMatrix.c
	> Author: Zhang Yuteng
	> Mail:
	> Created Time: 2019年05月23日 星期四 21时57分31秒
 ************************************************************************/

#include <stdlib.h>
#include "easyMatrix.h"

struct easyMatrix* setMatrix(DATA_TYPE * const a,struct easyMatrix* c) {
    uint8 x = c->rows;
    uint8 y = c->cols;
    int t = x*y;
    for(int i=0;i<t;++i) {
        c->element[i] = a[i];
    }
    return c;
}

struct easyMatrix* copyMatrix(struct easyMatrix* const a,struct easyMatrix* c) {
    int t = a->rows*a->cols;
    for(int i=0;i<t;++i) {
        c->element[i] = a->element[i];
    }
    return c;
}

struct easyMatrix* transMatrix(struct easyMatrix* const a,struct easyMatrix* c) {
    int index = 0;
    int index_src = 0;
    for(uint8 ii=0;ii<a->cols;++ii) {
        index_src=ii;
        for(uint8 jj=0;jj<a->rows;++jj) {
            //c->element[index] = a->element[jj*a->cols+ii];
            c->element[index] = a->element[index_src];
            index++;
            index_src+=a->cols;
        }
    }
    return c;
}

struct easyMatrix* leftMatrix(uint8 x_i,uint8 y_i, struct easyMatrix* const in, struct easyMatrix* out) {
    int index = 0;
    int index_src = 0;
    uint8 x =in->rows;
    uint8 y =in->cols;
    for(uint8 kk=0;kk<x;++kk) {
        for(uint8 ww=0;ww<y;++ww) {
            if(!(kk==x_i||ww==y_i)) {
                //out->element[index] = in->element[kk*y+ww];
                out->element[index] = in->element[index_src];
                index++;
            }
            index_src++;
        }
    }
    return out;
}
struct easyMatrix* adjMatrix(struct easyMatrix* const in, struct easyMatrix* out) {
    int index = 0;
    uint8 x = in->rows;
    uint8 y = in->cols;
    CREATE_DYNAMIC_MATRIX_ONHEAP(x-1,y-1,ret,NULL);
    signed char sign1 = 1;
    signed char sign2 = 1;
    for(uint8 ii=0;ii<x;++ii) {
        sign2 = sign1;
        index = ii;
        for(uint8 jj=0;jj<y;++jj) {
            leftMatrix(ii,jj,in,ret);
            //out->element[jj*y+ii] = sign2*detMatrix(ret);
            out->element[index] = sign2*detMatrix(ret);
            sign2 = - sign2;    
            index+=y;
        }
        
        sign1 = - sign1;
    }
    DELETE_DYNAMIC_MATRIX(ret);
    return out;
}

DATA_TYPE invMatrix(struct easyMatrix *const in , struct easyMatrix * out) {
    adjMatrix(in,out);
    DATA_TYPE scale = detMatrix(in);
    if(scale<1e-5&&scale>-1e-5) return 0.0;
    scale = 1/scale;
    scaleMatrix(scale,out,out);
    return scale;
}

DATA_TYPE detMatrix(struct easyMatrix* const in) {
    uint8 x = in->rows;
    uint8 y = in->cols;
    if(x!=y) return 0;
    if(x==0 ) return 0;
    if(x==1 ) return in->element[0];
    DATA_TYPE *a =in->element;
    if(x==2) return(a[0]*a[3]-a[1]*a[2]);
    DATA_TYPE result = 0;
    signed char sign = 1;
    CREATE_DYNAMIC_MATRIX_ONHEAP(x-1,y-1,ret,NULL);
    for(uint8 i=0;i<x;++i) {
        leftMatrix(0,i,in,ret);
        result += sign*a[i]*detMatrix(ret);
        sign = - sign;
    }
    DELETE_DYNAMIC_MATRIX(ret);
    return result;
}

struct easyMatrix* addMatrix(struct easyMatrix* const a, struct easyMatrix* const b, struct easyMatrix* c) {
    struct easyMatrix* obj = (struct easyMatrix*)a;
    int t = obj->rows*obj->cols;
    for(int i=0;i<t;++i) {
        c->element[i] = obj->element[i]+b->element[i];
    }
    return c;
}

struct easyMatrix* subMatrix(struct easyMatrix* const a, struct easyMatrix* const b, struct easyMatrix* c) {
    struct easyMatrix* obj = (struct easyMatrix*)a;
    int t = obj->rows*obj->cols;
    for(int i=0;i<t;++i) {
        c->element[i] = obj->element[i]-b->element[i];
    }
    return c;
}

struct easyMatrix* scaleMatrix(DATA_TYPE scale, struct easyMatrix* const a, struct easyMatrix* b) {
    int t = a->cols*a->rows;
    for (int i = 0;i<t;++i) {
        b->element[i] = a->element[i]*scale;
    }
    return b;
}

struct easyMatrix* multiMatrix(struct easyMatrix* const a,struct easyMatrix* const b, struct easyMatrix* c) {
    if(NULL==c) return NULL;
    if(c == a || c == b) return NULL;
    int count = 0;
    int t_cnt = 0;
    int z_cnt = 0;
    uint8 x = a->rows;
    uint8 y = a->cols;
    uint8 z = b->cols;
    for(uint8 i = 0;i<x;++i) {
        for(uint8 k = 0;k<z;++k) {
            c->element[count] = 0;
            z_cnt = 0;
            for(uint8 j = 0;j<y;++j) {
                c->element[count] += a->element[t_cnt+j]*b->element[z_cnt+k];
                z_cnt += z;
            }
            count++;
        }
        t_cnt+=y;
    }
    return c;
}

struct easyMatrix* zerosMatrix(struct easyMatrix* e) {
    if(e->rows != e->cols) return NULL;
    int t = e->cols*e->rows;
    for(int i=0;i<t;++i) {
        e->element[i] = 0;
    }
    return e;
}

struct easyMatrix* eyesMatrix(struct easyMatrix* e) {
    if(e->rows != e->cols) return NULL;
    zerosMatrix(e);
    int index = 0;
    for(uint8 i=0;i<e->rows;++i) {
        e->element[index] = 1.0;
        index+=(e->cols);
        ++index;
    }
    return e;
}

void dumpMatrix(struct easyMatrix* const e) {
    int count = 0;
    uint8 x = e->rows;
    uint8 y = e->cols;
    printf("cols is:%d, rows is:%d\n",x,y);
    for(uint8 i = 0;i<x;++i) {
        for(uint8 j = 0;j<y;++j) {
            printf("%8f,",e->element[count]);
            ++count;
        }
        printf("\n");
    }
    return;
}

