function [sens,P, b_mat,y] = stavenga(lmax,nbands,w_inter, plotit)
%Reconstruct spectral sensitivy of A1 photoreceptor using Stanvenga 1993
%template
%   [SENS] = STAVENGA (LMAX, NBANDS) Returns the spectral sensitivity
%   curve(s) of an A1 pigmment photoreceptor with N bands as specified by the
%   scalar NBANDS. NBANDS contains a scalar number between 1 and 3
%   indicating the number of bands included in the curve: 1 only alpha
%   band, 2 alpha band + beta band or 3 alpha + beta + gamma bands.
%
%   The matix LMAX holds the lambda max values for each one of the bands to
%   be calculated. If the lambda max value is known and NBANDS > 1 the
%   code assumes the default values of 340 and 276 nm for the beta and
%   gamma bands respectively. 
%
%   LMAX can be a row vector or a matrix. In the former case the lambda max
%   values are entered as lambda max alpha, lambda max beta and lambda max
%   gamma. If LMAX is a n-by-3 matrix, STAVENGA returns the spectral
%   sensitivity for each one of the n photoreceptors.
%
%  [SENS, P] = STANVENGA (LMAX, NBANDS) Returns a n-by-15 matrix holding
%  the paramters used for calculating the spectral sensitivity curves. The
%  columns of P hold: lambda max_-lpha, A-alpha, a0-alpha, a1-alpha,   
%  a2-alpha, lambda max-beta, A-beta, a0-beta, a1-beta, a2-beta, 
%  lambda max-gamma, A-gamma, a0-gamma, a2-gamma. Refer to the paper by
%  Stavenga et.al. 1993 for the meaning of these parameters.
%
% [SENS, P, B_MAT] = As above but also reuturns the cell BANDS_MAT
% containing the alpha,beta, and gamma bands for each one of the n
% photorecptors defined in the LMAX matrix.
%
% The optional scalar PLOTIT serves as a flag to plot the spectral
% sensitivity curve in SENS. If PLOTIT is omitted or ~= from 0 no plot is
% produced.

P = zeros(size(lmax,1),15); %Parameter matrix one row per curve
b_mat = cell(size(lmax,1),1);
% x = 300:5:830; x=x';
x = 300:w_inter:830; x=x'; %wavelength vector to evaluate template
sens = zeros(size(x,1),(size(lmax,1)+1));
for ii = 1:size(lmax,1)
    y = ii + 1;
    P(ii,1) = lmax(ii,1);
    P(ii,2) = 1;
    P(ii,3) = 380;
    P(ii,4) = 6.09;
    P(ii,5) = 3*((P(ii,4)^2))/8;
    if size(lmax,2) == 1
        P(ii,6) = 340;
    else
        P(ii,6) = lmax(ii,2);
    end
    P(ii,7) = 0.29;
    P(ii,8) = 247;
    P(ii,9) = 3.59;
    P(ii,10) = 3*((P(ii,9)^2))/8;
    if size(lmax,2) == 2
        P(ii,11) = 276; %Gamma band lmax
    else
        P(ii,11) = lmax(ii,3);
    end
    P(ii,12) = 1.99;
    P(ii,13) = 647;
    P(ii,14) = 23.4;
    P(ii,15) = 3*((P(ii,14)^2))/8;
    ind = [1;6;11];
    bands_mat = zeros(length(x),3);
    for jj = 1:size(ind,1)
        for kk = 1:size(x,1)
            bands_mat(kk,jj) = P(ii,(ind(jj,1)+1))*...
            exp(-P(ii,(ind(jj,1)+2))*((log10(x(kk,1)...
            /P(ii,(ind(jj,1)))))^2)*...
            (1+(P(ii,(ind(jj,1)+3))*(log10(x(kk,1)/...
            P(ii,(ind(jj,1))))))+(P(ii,(ind(jj,1)+4))...
            *(log10(x(kk,1)/P(ii,(ind(jj,1)))))^2)));
        end
        sens(:,y) = sum(bands_mat(:,1:nbands),2);
    end
    
    b_mat{ii} = bands_mat;
end
sens(:,1) = x;
if nargin == 4 && plotit == 1
plot(x,sens(:,2:size(sens,2)))
end
     
                
    
end

