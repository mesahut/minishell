# FİXED
- [x] echo $                                                      
- [x] echo $Home                                                  
- [x] export a="echo a" yazıldığında not valid hatası alıyoruz    
- [x] terminaldeyken entere bastığımızda core dump hatası alıyoruz
- [x] > asd execve field hatası alıyor dosya açması gerekli
- [x] >> asd execve field hatası alıyor dosya açması gerekli
- [x] export 1a="123" tanımlama hatası alıyor ama export as2="123" tanımlanabiliyor       (export kısmında bunu fixledim şu anda yorum satırında kontrol edip yorum satırından çıkarabilir)
- [x] env komutunun ardından herhangi birşey yazılırsa hata kodu verdirecez
- [x] lexerdaki quote checker kısmında hata alırsak hata çıktısı verdirilecek
- [x] cat < testfile
- [x] echo $? yok
- [x] lexerdaki quote checker kısmında hata alırsak hata çıktısı verdirilecek
- [x] echo test >  syntax error vermeli biz test bastırıyoruz
- [X]pipe bouzk aq
- [x] export a="cho a" yazdıktan sonra e$a yazarsak definitliy lost çıkıyor
- [x] echo test | heredoc gibi girdi penceresi açıyor biz test bastırıyoruz
- [x] | cat bizim çıktımız (null): command not found olması gereken syntax error
- [ ] mallocların hata durumları kontrol edilecek
- [ ] string.h tan çekilen fonksiyonlar ftler ile değiştirilecek
- [x] ls /nonexistent 2> /tmp/error_test cat /tmp/error_test testinde bizim çıktımız: ls: '/nonexistent' ögesine erişilemedi: Böyle bir dosya ya da dizin yok
                                                                                     ls: '2' ögesine erişilemedi: Böyle bir dosya ya da dizin yok
                                                                                     ls: 'cat' ögesine erişilemedi: Böyle bir dosya ya da dizin yok
                                                                   bash in çıktısı : ls: '/nonexistent' ögesine erişilemedi: Böyle bir dosya ya da dizin yok
1 export toplu giriş
herdoc ard arda çalışmıyor l
ls | pwd 
. hata
dasdasd | . | pwd çıktı = hata - hata - pwd
dsadasd errorları standart 2 ye yazdır 

env | grep PATH leak yiyor (child içinden kalıyor)

unset PATH yapinca komutlar calismaya devam ediyor etmemesi lazim fix

bir executable dosyanin pathi export edildiginde, her yerden calistirilabilir hale gelmesi gerekiyor. fix

env -i handle fix

bos komutlarda leak yiyor (enter atinca) fix

cat << 123 | cat << 456

cat << 123 echo asd

cat < $?t.txt leak sorunu

SIGQUIT handle ctrl + \

ctrl + c ve ctrl + \' de $? guncellenecek. 130 ve 131

genel olarak exit statuslerde sikinti var gibi.

exit 31 69 cikmamasi lazim.

./ leak 

"" leak
