type Color = [number, number, number];

/*
 * This code was shamelessly ~~stolen~~ borrowed from
 * https://stackoverflow.com/a/17243070 (CC BY-SA 3.0)
 */

export function RGBtoHSV(color: Color): Color
export function RGBtoHSV(red: Number, green: Number, blue: Number): Color
export function RGBtoHSV(...args: any[]): Color {
    let r = 0, g = 0, b = 0;

    if (args.length === 1) {
        [r, g, b] = args[0] as Color;
    } else {
        [r, g, b] = args as Color;
    }

    let max = Math.max(r, g, b),
        min = Math.min(r, g, b),
        d = max - min,
        h = 0,
        s = (max === 0 ? 0 : d / max),
        v = max / 255;

    switch (max) {
        case min: h = 0; break;
        case r: h = (g - b) + d * (g < b ? 6 : 0); h /= 6 * d; break;
        case g: h = (b - r) + d * 2; h /= 6 * d; break;
        case b: h = (r - g) + d * 4; h /= 6 * d; break;
    }

    return [h, s, v];
}

export function HSVtoRGB(col: Color): Color
export function HSVtoRGB(hue: number, saturation: number, value: number): Color
export function HSVtoRGB(...args: any[]): Color {
    let h = 0,
        s = 0,
        v = 0;

    if (args.length === 1) {
        [h, s, v] = args[0] as Color;
    } else {
        [h, s, v] = args as Color;
    }
    let r = 0, g = 0, b = 0,
        i, f, p, q, t;
    i = Math.floor(h * 6);
    f = h * 6 - i;
    p = v * (1 - s);
    q = v * (1 - f * s);
    t = v * (1 - (1 - f) * s);
    switch (i % 6) {
        case 0: r = v, g = t, b = p; break;
        case 1: r = q, g = v, b = p; break;
        case 2: r = p, g = v, b = t; break;
        case 3: r = p, g = q, b = v; break;
        case 4: r = t, g = p, b = v; break;
        case 5: r = v, g = p, b = q; break;
    }
    return [
        Math.round(r * 255),
        Math.round(g * 255),
        Math.round(b * 255)
    ];
}

export function toCssHSV(color: Color): string {
    return `hsv(${color[0] * 360}, ${color[1] * 100}%, ${color[2] * 100}%)`;
}

export function toCssRGB(color: Color): string {
    return `rgb(${color[0]}, ${color[1]}, ${color[2]})`;
}