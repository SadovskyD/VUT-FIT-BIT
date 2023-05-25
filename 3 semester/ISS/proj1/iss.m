fprintf('1)\n');
[signal, fs] = audioread('xsadov06.wav');
signal = signal';
samples_length = length(signal);
MAX = max(signal);
MIN = min(signal);
fprintf('Délka signálu ve vzorcích: %f, v sekundách: %f [s].\n', samples_length, samples_length / fs);
fprintf('Maximalni hodnota: %f. Minimalni hodnota: %f.\n', MAX, MIN);
t = linspace(0, samples_length / fs, samples_length);
figure;plot (t,signal);
fprintf('2)\n');
SH = mean(signal);
s_ust = signal - SH;
s_norm =s_ust/max(abs(s_ust));
for cnt = 1:512:66253
r(:,(cnt-1)/512+1)=s_norm(cnt:cnt+1024);
end
t1024 = linspace(0, 1024 / fs, 1025);
figure;plot(t1024,r(:,10));
w=exp(-2*pi*1i/1024)
for j=0:1024
for k=0:1024
W(j+1,k+1)=w^(j*k);
end
end
s_dft=rot90(r(:,10))*W;
figure;plot(linspace(0, fs/2, length(s_dft(1:512))),abs(s_dft(1:512)));
dft_r=fft(r);
G=10*log10(abs(dft_r).^2);
figure;imagesc([0 samples_length/fs],[0 8000],G(1:513,:));
t=(0:1/fs:samples_length/fs);
g_s=cos(2*pi*940*t)+cos(2*pi*1860*t)+cos(2*pi*2797*t)+cos(2*pi*3734*t);
figure;specgram(g_s);
audiowrite('4cos.wav',g_s,fs);
dots=[940 1875 2783 3720];
dots=exp(1i*2*pi*dots/fs);
dots=[dots conj(dots)];
kf=poly(dots);
tmp=[[0,0,0,0,0,0,0,0] signal];
for i=1:samples_length
out(i)=tmp(i+8)*kf(1)+tmp(i+7)*kf(2)+tmp(i+6)*kf(3)+tmp(i+5)*kf(4)+tmp(i+4)*kf(5)+tmp(i+3)*kf(6)+tmp(i+2)*kf(7)+tmp(i+1)*kf(8)+tmp(i)*kf(9);
end