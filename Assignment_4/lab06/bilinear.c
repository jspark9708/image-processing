

		NW = buffer[Y_Source * cols + X_Source];
		NE = buffer[Y_Source * cols + X_Source+1];
		SW = buffer[(Y_Source+1) * cols + X_Source];
		SE = buffer[(Y_Source+1) * cols + X_Source+1];

		EWweight = (x / x_scale) - (float)X_Source;
		NSweight = (y / y_scale) - (float)Y_Source;

		EWtop = NW + EWweight*(NE-NW);
		EWbottom = SW + EWweight*(SE-SW); 


 