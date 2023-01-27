#pragma once

struct Vec2
{
	float X;
	float Y;
};

struct Vec3
{
	float X;
	float Y;
	float Z;
};

struct Vec4
{
	float x, y, z, w;
};

struct ViewMatrix
{
	float Matrix[16];
};

void ConvertToRange(ImVec2& Point)
{
	Point.x /= 1920.0f;
	Point.x *= 2.0f;
	Point.x -= 1.0f;

	Point.y /= 1080.0f;
	Point.y *= 2.0f;
	Point.y -= 1.0f;
}

bool WorldToScreen2(Vec3 pos, ImVec2& screen, float* viewMatrix)
{
	Vec4 clipCoords;
	clipCoords.x = pos.X * viewMatrix[0] + pos.Y * viewMatrix[1] + pos.Z * viewMatrix[2] + viewMatrix[3];
	clipCoords.y = pos.X * viewMatrix[4] + pos.Y * viewMatrix[5] + pos.Z * viewMatrix[6] + viewMatrix[7];
	clipCoords.z = pos.X * viewMatrix[8] + pos.Y * viewMatrix[9] + pos.Z * viewMatrix[10] + viewMatrix[11];
	clipCoords.w = pos.X * viewMatrix[12] + pos.Y * viewMatrix[13] + pos.Z * viewMatrix[14] + viewMatrix[15];

	if (clipCoords.w < 0.1f)
		return false;

	Vec3 NDC;
	NDC.X = clipCoords.x / clipCoords.w;
	NDC.Y = clipCoords.y / clipCoords.w;
	NDC.Z = clipCoords.z / clipCoords.w;

	screen.x = (1920 / 2 * NDC.X) + (NDC.X + 1920 / 2);
	screen.y = -(1080 / 2 * NDC.Y) + (NDC.Y + 1080 / 2);

	return true;
}

bool WorldToScreen(Vec3& vecOrigin, ImVec2& VecScreen, float* Matrix)
{
	VecScreen.x = vecOrigin.X * Matrix[0] + vecOrigin.Y * Matrix[1] + vecOrigin.Z * Matrix[2] + Matrix[3]; //correct 
	VecScreen.y = vecOrigin.X * Matrix[4] + vecOrigin.Y * Matrix[5] + vecOrigin.Z * Matrix[6] + Matrix[7]; //correct
	float W = vecOrigin.X * Matrix[12] + vecOrigin.Y * Matrix[13] + vecOrigin.Z * Matrix[14] + Matrix[15]; //correct

	if (W < 0.01f)
	{
		return false;
	}
	
	//NDC = normalized device coords
//	Vec2 NDC;
	ImVec2 NDC;
	
	NDC.x = VecScreen.x / W;//correct
	NDC.y = VecScreen.y / W;//correct

	VecScreen.x = (1920 / 2 * NDC.x) + (NDC.x + 1920 / 2); //correct
	VecScreen.y = (1080 / 2 * NDC.y) + (NDC.y + 1080 / 2); //correct

	ConvertToRange(VecScreen); //correct

	return true; //correct
}