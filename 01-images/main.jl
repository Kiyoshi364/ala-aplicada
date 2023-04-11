#! julia -L
# julia> test1()
# ...

should_update = false :: Bool
if should_update
    # import Pkg; Pkg.add("Images")
    # import Pkg; Pkg.add("ImageShow")
    import Pkg; Pkg.add("Colors")
end
# using Images, ImageShow
using Colors

# globals

iota5 = [ 0 1 2 3 4 ]
iota5x5 = [
    [ 0 1 2 3 4 ]
    [ 5 6 7 8 9 ]
    [ 10 11 12 13 14 ]
    [ 15 16 17 18 19 ]
    [ 20 21 22 23 24 ]
]

lerp(a :: T, b :: T, t :: Float64) where {T} =
    (1 - t) * a + t * b

color_pallet = [
    RGB(0, 0, 0)
    RGB(1, 0, 0)
    RGB(1, 1, 0)
    RGB(0, 1, 0)
    RGB(0, 1, 1)
    RGB(0, 0, 1)
    RGB(1, 0, 1)
    RGB(1, 1, 1)
]

color_matrix = [
    [ RGB(r, g, lerp(r, g, 0.5)) for r in 0:0.1:1 ]
    for g in 0:0.1:1
]

# Helper

function run(func, inputs...)
    println("= running ", Symbol(func), ", on")
    for input in inputs
        print("> ")
        display(input)
    end
    println("= output:")
    display(func(inputs...))
    println("\n=====================")
end

function run_broadcast(func, inputs...)
    println("= running ", Symbol(func), ", on")
    for input in inputs
        print("> ")
        display(input)
    end
    println("= output:")
    display(func.(inputs...))
    println("\n=====================")
end

function runln(func, inputs...)
    run(func, inputs...)
    println()
end

function run_broadcastln(func, inputs...)
    run_broadcast(func, inputs...)
    println()
end

# 1 - Função Reverse
# Nota: não sei escrever mais bonito que isso
reverse(array :: Array{T}, axis :: Int = 1) where {T} =
    if axis == 1
        array[:, size(array, 2):-1:1]
    elseif axis == 2
        array[size(array, 1):-1:1, :]
    else
        assert(False, "reverse axis is too big") 
    end

function test1()
    runln(reverse, iota5)
    runln(reverse, iota5, 2)
    runln(reverse, iota5x5)
    runln(reverse, iota5x5, 2)
end

function test1_1()
    runln(reverse, color_pallet, 2)
    runln(reverse, color_matrix)
    runln(reverse, color_matrix, 2)
end

# 2 - Criar uma imagem usando 3 funções
function compose_image(image :: Matrix{T}) :: Matrix{T} where {T}
    fR(x) = 0.5 * x
    fL(x) = 2 * x
    imageR = fR.(reverse(image, 1)) :: Matrix{T}
    imageL = fL.(reverse(image, 2)) :: Matrix{T}
    imageRL =  0.5 * image
        + 0.5 * reverse(reverse(image, 1), 2) :: Matrix{T}
    [
        image imageR ;
        imageL imageRL
    ]
end

function test2()
    runln(compose_image, convert.(Float64, iota5x5))
end

# 3 - Broadcast

# Também chamada de 'const' ou 'K'
# T -> U -> T
function kestrel(a :: T) where {T}
    function inner(b :: U) :: T where {U}
        a
    end
end

broadcast_1f = kestrel(1.0 :: Float64)
broadcast_red = kestrel(RGB(1, 0, 0) :: RGB)

function test3()
    run_broadcastln(broadcast_1f, iota5x5)
    run_broadcastln(broadcast_red, iota5x5)
end

# 4 - Imagem colorida para preto e branco

extend_to_color(x) :: RGB = RGB(x, x, x)

extend_red(c :: RGB) :: RGB = extend_to_color(c.r)
extend_green(c :: RGB) :: RGB = extend_to_color(c.g)
extend_blue(c :: RGB) :: RGB = extend_to_color(c.b)

to_lumHSL(c :: RGB) :: RGB =
    extend_to_color(
        0.5 * max(c.r, c.g, c.b)
        + 0.5 * min(c.r, c.g, c.b)
    )

to_lumAvg(c :: RGB) :: RGB =
    extend_to_color(
        (c.r / 3) + (c.g / 3) + (c.b / 3)
    )

to_black_white(image, map = to_lumHSL) = map.(image)

function test4()
    runln(to_black_white, color_pallet)
    runln(to_black_white, color_pallet, extend_red)
end

# 5 - Zoom!!!
function zoom(
        image :: Array{T},
        factor_tuple :: Tuple{Number, Number}
        ) where {T}
    image_size = size(image)
    target_size = max.(
        image_size,
        ((image_size .- 1) .* factor_tuple) .+ 1
        )
    println("target_size is ", target_size)
    zoomed_image = Array{T}(undef, target_size)
    # This code assumes that image has 2 dimentions
    @assert ndims(image) == 2 "image should have 2 dimensions"

    # Extend lines
    for j in 0:target_size[2]-1
        src_j1 = floor(Int, j / factor_tuple[2])
        src_j2 = ceil(Int, j / factor_tuple[2])
        t = (j % factor_tuple[2]) / factor_tuple[2]
        for i in 0:target_size[1]-1
            src_i = floor(Int, i / factor_tuple[1])
            if src_i != ceil(Int, i / factor_tuple[1])
                zoomed_image[i+1, j+1] = -1
                continue
            end
            a = image[src_i+1, src_j1+1]
            b = image[src_i+1, src_j2+1]
            zoomed_image[i+1, j+1] = lerp(a, b, t)
        end
    end

    # Uncomment to see midstep
    # display(zoomed_image)

    # Extend colums
    for i in 0:target_size[1]-1
        src_i1 = floor(Int, i / factor_tuple[1]) * factor_tuple[1]
        src_i2 = min(
            target_size[1] - 1,
            ceil(Int, i / factor_tuple[1]) * factor_tuple[1]
            )
        t = (i % factor_tuple[1]) / factor_tuple[1]
        for j in 0:target_size[2]-1
            if src_i1 == src_i2
                continue
            end
            a = zoomed_image[src_i1+1, j+1]
            b = zoomed_image[src_i2+1, j+1]
            zoomed_image[i+1, j+1] = lerp(a, b, t)
        end
    end

    zoomed_image
end

function test5()
    runln(zoom, convert.(Float64, iota5), (2,2))
    runln(zoom, convert.(Float64, iota5x5), (2,2))
    runln(zoom, convert.(Float64, iota5x5), (1,3))
    runln(zoom, convert.(Float64, iota5x5), (2,3))
end
