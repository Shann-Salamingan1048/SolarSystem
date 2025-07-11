#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
uniform sampler2D earth;

void main()
{
    vec4 texColor = texture(earth, TexCoord);
    if(texColor.a < 0.1) // Discard nearly transparent pixels
        discard;
    FragColor = texColor;
}