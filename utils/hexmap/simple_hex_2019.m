function [c_coord, Gc, u, b, g] = simple_hex_2019(R,sens,bck,SPD)
%SIMPLE_HEXAGON simplified calculations for Hexagon model to work as an
%internal function. It returns x-y coordinate pairs and green contrast (Gc)
%valuesfor each 36-by-1 reflectance spectrum (between 0 and 1) stored in 
%matrix R. Matrix SENS holds the spectral sensitivity of the hymenopteran 
%observer stored as a 36-by-3 matrix whose columns hold the values for the
%UV, blue and green photoreceptors. The column vector BCK holds the 
%reflectance of the adaptation background within 300 to 650 nm at 10 nm
%intervals (ALG). Finally SPD is the spectral distribution of the
%illuminant expressed in quantum units: umol/m^2/s/nm. 
%

if size(R,1) ~= size(sens,1) || size(R,1) ~= size(bck,1) || ...
        size(R,1) ~= size(SPD,1) || size(sens,1) ~= size(bck,1) || ...
        size(sens,1) ~= size(SPD,1) || size(bck,1) ~= size(SPD,1)
    error('All vectors should be 36 - by -1') %Checks size of input
end
SPD = diag(SPD);
SI = SPD*sens;
Ps = R'*SI;
Pbck = bck'*SI;
vKs = 1./Pbck;
P = zeros(size(Ps,1),3);
for ii = 1:3
    P(:,ii) = Ps(:,ii)*vKs(:,ii); %Catches per receptor per sample
end
E = P./(P+1); %Nasha and Rushton normalisation Eq.
c_coord=zeros(size(E,1),2);
for ii = 1:size(E,1)
    c_coord(ii,2) = (1*E(ii,2))-(0.5*E(ii,1))-(0.5*E(ii,3));
    c_coord(ii,1) = (sqrt(3)/2)*(E(ii,3)-E(ii,1));
end
Gc = abs(E(:,3) - 0.5); %Green contrast is defined as the absolute
                        %deviation of the difference between E(G) and 0.5
                        %equivalent to the adaptation background.
u=E(:,1);
b=E(:,2);
g=E(:,3);
end

