## Cross co-relation and convolution opreators. 
    Impulse signal double flips on filter application.
    Convolution nullifies this double flip.
    T(i,j)= $u$v (H(u,v)*I(i-u,j-u))  where $ is summation opreator
    T is the transformed image, H is the filter, and I is the input image.
    So convolution is cross correleation of double flipped input. This are linear
    shift invarient opreator, Linearity means Image I with filters h0 and h1 when
    applied are distributive ie, I(h0 + h1) = I*h0 + I*h1, also known as the
    super position principle. The other property is called shift invarience :
    shifting (or translating) a signal commutes with applying the opreator.
    T(i,j) = H(i+k,j+l) -> f(g(i,j)) = f(h(i+k,j+l)), In layman terms the effect
    of the opreator is the same no matter where in the image, ie it is invarient
    of the image location. Convolution (denoted by * opreator) is commutative  a*b = b*a ( no diff btwn filter and signal/img) a*(b*c) = (a*b)*c helps in precomputations and reducing computation time, scalar : ka*b = a*kb = k*(a*b). 
    An important optimization uses property of seperablity usually for image of size nxn, and a filter size of k (usually 3) we require k^2 x n^2 computatations over the entire image, making it very expensive, seperablity allows us in some cases to bring this computation down to (2k * n^2) by performing 1d horizontal convolution followd by 1d vertial convolution having the same effect as the k^2 computations per pixel at end.An important optimization uses property of seperablity usually for image of size nxn, and a filter size of k (usually 3) we require k^2 x n^2 computatations over the entire image, making it very expensive, seperablity allows us in some cases to bring this computation down to (2k * n^2) by performing 1d horizontal convolution followd by 1d vertial convolution having the same effect as the k^2 computations per pixel at end. To formalize a separable kernal is one, which can be written as vector product of two 1d kernals v and h^t ( h^t is transpose of h) and by convolution properties, we can reduce computations. 
            Example 
                1/16[ [1 2 1 ]
                      [2 4 1 ]
                      [1 2 1 ] ]
                = v = h = 1/4 [1 2 1 ]
    to check if a given filter is separable check if it can be checked by K = DU Singular value decomposition. 
    
    * When doing convolution we lose out on border information, to mitigate that we can perform padding, a naive padding would be 0 padding. 
