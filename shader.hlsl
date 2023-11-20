struct vs_in {
	float3 position : POSITION;
	float3 color 	: COLOR;
};

struct vs_out {
	float4 position : SV_POSITION;
	float4 color	: COLOR;
};

vs_out vs_main(vs_in input) {
	vs_out output;

	output.position = float4(input.position, 1.0f);
	output.color = float4(input.color, 1.0f);

	return output;
}

float4 ps_main(vs_out input) : SV_TARGET {
	return input.color;
}
