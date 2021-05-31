# POBR - projekt

Autor: Sławomir Nikiel

Tematem projektu jest implementacja programu rozpoznającego numery na drewnianych klockach. Rozpoznawanie zrealizowane jest za pomocą klasycznych metod przetwarzania obrazów oraz klasyfikacji obiektów. Sam program zaimplementowany jest w języku C++. Wszystkie kluczowe algorytmy  wykorzystywane w rozwiązaniu zawarte są w kodzie źródłowym, wspierają je struktury danych zawarte w bibliotece OpenCV.

## Rozwiązanie

Rozwiązanie przedstawionego problemu jest procesem wieloetapowym. Można z niego wydzielić dwie podstawowe fazy. Fazę poprawy jakości i progowania oraz fazę segmentacji i rozpoznawania. Ogólny przepływ obrazu w aplikacji przedstawiony jest na poniższym diagramie. 

![przeplyw](C:\Users\snk\Documents\Visual Studio 2019\Solutions\BlocksDetector\sprawozdanie.assets\przeplyw.png)

### Poprawa jakości i progowanie

Poprawa jakości wejściowego obrazu odbywa się z użyciem filtracji rankingowej. Użycie tego rodzaju filtra ma na celu zmniejszenie wpływu niedoskonałości oraz naturalnych cech materiału, czyli drewna, z którego wykonane są klocki. Po przefiltrowaniu obrazu poddawany jest on progowaniu. Odbywa się ono w przestrzeni barw HLS i obejmuje wszystkie jego trzy kanał. Progowanie ma na celu wygenerowanie maski binarnej wyodrębniającej poszukiwane cyfry. Niestety sama ta metoda jest niewystarczająca i została wsparta inną w dalszej fazie przetwarzania. Następnie maska przepuszczana jest przez trzy filtry morfologiczne, podwójną dylację oraz erozję. W ten sposób uzyskiwany jest obraz binarny o większej ciągłości. Tak przygotowana maska przekazywana jest do kolejnego etapu przetwarzania.

### Segmentacja i rozpoznawanie

Na podstawie wcześniej wspomnianej maski oraz kolorowego obrazu otrzymanego po filtracji rankingowej wykonywana jest segmentacja obrazu. Odbywa się ona algorytmem FloodFill z uwzględnieniem kanału V przestrzeni barw HSV. Segmenty wydzielane są na podstawie spójności obszarów binarnej maski oraz progowania wspomnianego kanału V pikseli. Taka metoda pozwala wyodrębnić poszczególne cyfry nawet w przypadku, gdy nie zostały one całkowicie odseparowane za pomocą maski. Poniższy obraz przedstawia wygenerowaną w opisany sposób mapę segmentów.

<img src="C:\Users\snk\Documents\Visual Studio 2019\Solutions\BlocksDetector\sprawozdanie.assets\segmentMap0.png" alt="segmentMap0" style="zoom:50%;" />

Podczas generowania mapy tworzone są obiekty poszczególnych segmentów, które następnie przekazywane są do fazy rozpoznawania. W fazie rozpoznawania segmenty poddawane są klasyfikacji na podstawie odpowiednich zestawów momentów geometrycznych. W ten sposób rozpoznawane są poszczególne cyfry na klockach. W celu rozpoznania liczby 10 analizowane są wszystkie pary cyfr 1 i 0. Analiza ta polega na łączeniu chmur punktów zadanych segmentów, a następnie sklasyfikowaniu, za pomocą momentów geometrycznych, tak nowopowstałego segmentu pod kątem reprezentowania liczby 10. Po wykonaniu wszystkich opisanych wcześniej etapów przetwarzania zadanego obrazu uzyskuje się wystarczające dane do zaprezentowania wyników, których przykład znajduje się na kolejnym obrazie.

<img src="C:\Users\snk\Documents\Visual Studio 2019\Solutions\BlocksDetector\sprawozdanie.assets\out1.png" alt="out1" style="zoom:50%;" />