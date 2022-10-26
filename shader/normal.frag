#version 330 core

out vec4 color;

in float vertexDepth;

uniform vec2 zNearFar;
uniform int pDepthMap;

void main()
{
    if (pDepthMap == 1)
    {
        // Normalize depth so at least we get to see something
        float depth = gl_FragCoord.z * 2.0 - 1.0;
        float depthLin = ((2.0 * zNearFar.x * zNearFar.y) / (zNearFar.y + zNearFar.x - depth * (zNearFar.y - zNearFar.x))) / zNearFar.y;

        color = vec4(depthLin, depthLin, depthLin, 1.0);    
    }
    else if (pDepthMap == 2)
    {
        color = vec4(1.0, 0.5, 0.0, 1.0); 
    }
    else if (pDepthMap == 3) 
    {
        // float depthNor = (depth - zNearFar.x) / (zNearFar.y - zNearFar.x);
        color = vec4(vertexDepth, vertexDepth, vertexDepth, 1.0);
        if (vertexDepth < -0.01)
        {
            color = vec4(-vertexDepth / 200.0, -vertexDepth / 200.0, -vertexDepth / 200.0, 1.0);
        }
    }
    else
    {
        float depth = gl_FragCoord.z;
        color = vec4(depth, depth, depth, 1.0);
    }
}
