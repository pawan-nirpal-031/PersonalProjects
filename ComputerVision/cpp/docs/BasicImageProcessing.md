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
    of the image location. Convolution (denoted by * opreator) is commutative  a*b = b*a ( no diff btwn filter and signal/img) a*(b*c) = (a*b)*c helps in precomputations and reducing computation time, scalar : ka*b = a*kb = k*(a*b),Identity : 