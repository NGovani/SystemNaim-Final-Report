int fut(int x){
    return (2 * x) - 20;
}


int hls_test_func_a(int h, int n){
    int i = 0;
    int sum = 0;
    int x = 0;
    for(i = 0; i < n; i = i + 1){
        x = 50 + (2*i- 1) * h;
        sum = sum + fut(x);
    }
    return sum;
}

int hls_test_func_b(int h, int n){
    int i = 0;
    int sum = 0;
    int x;
    for(i = 1; i < n; i = i + 1){
        x = 50 + 2 * i * h;
        sum = sum + fut(x);
    }
    return sum;
}


int hls_test_top(int n, int fin){
    int from = 50;
    int h = (fin - from) / n;
    int holda;
    int holdb;
    int funcA;
    int funcB;

    int ret;
    holda = hls_test_func_a(h, n/2);
    holdb = hls_test_func_b(h, (n/2 - 1));

    holda = 4*holda;
    holdb = 2*holdb;
    funcA = fut(from);
    funcB = fut(fin);
    ret = (h/3) * (holda + holdb + funcA + funcB);    
    return ret;
}
