YELLOW='\033[1;33m'
BLUE='\033[1;34m'
GREEN='\033[1;32m'
RESET='\033[0m'

echo -e "${YELLOW}Mahmut Derleyici 1.0.0 V Başlatılıyor...${RESET}"
(make re > /dev/null 2>&1 && make clean > /dev/null 2>&1 && clear > /dev/null 2>&1) &
MAKE_PID=$!
LOADING_STEP=0
while kill -0 $MAKE_PID 2>/dev/null; do
    DOTS=$((LOADING_STEP%4))
    echo -ne "${BLUE}loading"; for j in $(seq 1 $DOTS); do echo -n "."; done; echo -ne "   \r${RESET}"; sleep 0.5;
    LOADING_STEP=$((LOADING_STEP+1))
done
echo -e "${GREEN}Mahmut Derleyici 1.0.0 V Tamamlandı!${RESET}"