-- Copyright (c) 2017-2024, Mudita Sp. z.o.o. All rights reserved.
-- For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

-- Create required tables
CREATE TABLE IF NOT EXISTS quote_languages (
    lang_id INTEGER NOT NULL,
    lang_name TEXT NOT NULL UNIQUE,
    PRIMARY KEY (lang_id)
);

CREATE TABLE IF NOT EXISTS quote_table (
    quote_id INTEGER NOT NULL,
    quote TEXT NOT NULL,
    author TEXT NOT NULL,
    quote_lang INTEGER NOT NULL,
    PRIMARY KEY (quote_id)
);

-- Fill quote languages table
INSERT OR REPLACE INTO quote_languages (lang_id, lang_name) VALUES
    (1, 'English'),
    (2, 'Polski'),
    (3, 'Français'),
    (4, 'Español'),
    (5, 'Deutsch');


-- NOTE:
-- If any new quote is to be added here - it was determined during design
-- process that its length should not exceed 58 characters and author name's
-- length should not exceed 12 characters. Otherwise it will not properly
-- fit on Harmony's display. Also make sure that it doesn't contain any
-- unsupported characters.

-- Fill quote table with English quotes
INSERT OR REPLACE INTO quote_table (quote_id, quote, author, quote_lang) VALUES
    (1, 'The simplest things are often the truest.', 'Richard Bach', 1),
    (2, 'Slow down, simplify and be kind.', 'Naomi Judd', 1),
    (3, 'It is always the simple that produces the marvelous.', 'Amelia Barr', 1),
    (4, 'But remember, there''s no greater gift than the present.', 'Dan Santat', 1),
    (5, 'The wound is the place where the Light enters you.', 'Rumi', 1),
    (6, 'Where there is ruin, there is hope for a treasure.', 'Rumi', 1),
    (7, 'Set your life on fire. Seek those who fan your flames.', 'Rumi', 1),
    (8, 'Wherever you are, and whatever you do, be in love.', 'Rumi', 1),
    (9, 'Reason is powerless in the expression of Love.', 'Rumi', 1),
    (10, 'Only from the heart can you touch the sky.', 'Rumi', 1),
    (11, 'Do not feel lonely, the entire universe is inside you.', 'Rumi', 1),
    (12, 'Shine like the whole universe is yours.', 'Rumi', 1),
    (13, 'What matters is how quickly you do what your soul directs.', 'Rumi', 1),
    (14, 'Let silence take you to the core of life.', 'Rumi', 1),
    (15, 'The only lasting beauty is the beauty of the heart.', 'Rumi', 1),
    (16, 'Try something different. Surrender.', 'Rumi', 1),
    (17, 'Peace comes from within. Do not seek it without.', 'Buddha', 1),
    (18, 'Doubt everything. Find your own light.', 'Buddha', 1),
    (19, 'There is no path to happiness: happiness is the path.', 'Buddha', 1),
    (20, 'Do not look for a sanctuary in anyone except your self.', 'Buddha', 1),
    (21, 'The greatest prayer is patience.', 'Buddha', 1),
    (22, 'Wear your ego like a loose fitting garment.', 'Buddha', 1),
    (23, 'He is able who thinks he is able.', 'Buddha', 1),
    (24, 'Remembering a wrong is like carrying a burden on the mind.', 'Buddha', 1),
    (25, 'Be where you are; otherwise you will miss your life.', 'Buddha', 1),
    (26, 'Be vigilant; guard your mind against negative thoughts.', 'Buddha', 1),
    (27, 'Be greatly aware of the present.', 'Buddha', 1),
    (28, 'The mind is everything. What you think, you become.', 'Buddha', 1),
    (29, 'The teaching is simple. Do what is right. Be Pure.', 'Buddha', 1),
    (30, 'A jug fills drop by drop.', 'Buddha', 1),
    (31, 'Work out your own salvation. Do not depend on others.', 'Buddha', 1),
    (32, 'Contentment is the greatest wealth.', 'Buddha', 1),
    (33, 'The less you have, the less you have to worry about.', 'Buddha', 1),
    (34, 'Be your own lamp.', 'Buddha', 1),
    (35, 'Patience is the highest asceticism.', 'Buddha', 1),
    (36, 'Live every act fully, as if it were your last.', 'Buddha', 1),
    (37, 'Above all, do not lose your desire to walk.', 'Buddha', 1),
    (38, 'Is it timely? Is it necessary? Is it kind?', 'Buddha', 1),
    (39, 'Make things as simple as possible but no simpler.', 'Einstein', 1),
    (40, 'Even the darkest night will end and the sun will rise.', 'Victor Hugo', 1),
    (41, 'To find fault is easy; to do better may be difficult.', 'Plutarch', 1),
    (42, 'The greater the obstacle, the more glory in overcoming it.', 'Molière', 1),
    (43, 'He who has begun is half done. Dare to be wise. Begin.', 'Horace', 1),
    (44, 'Wonder is the beginning of wisdom.', 'Socrates', 1),
    (45, 'A journey of a thousand miles begins with a single step.', 'Lao Tzu', 1),
    (46, 'Adventure is worthwhile.', 'Aristotle', 1),
    (47, 'Wherever my travels may lead, paradise is where I am.', 'Voltaire', 1),
    (48, 'The sun is new each day.', 'Heraclitus', 1),
    (49, 'Be happy for this moment. This moment is your life.', 'Omar Khayyam', 1),
    (50, 'The soul never thinks without a picture.', 'Aristotle', 1),
    (51, 'Choose rather to be strong of soul than strong of body.', 'Pythagoras', 1),
    (52, 'A true friend is one soul in two bodies.', 'Aristotle', 1),
    (53, 'Whatever satisfies the soul is truth.', 'Walt Whitman', 1),
    (54, 'Music in the soul can be heard by the universe.', 'Lao Tzu', 1),
    (55, 'Life''s greatest happiness is to be convinced we are loved.', 'Victor Hugo', 1),
    (56, 'Nature does not hurry, yet everything is accomplished.', 'Lao Tzu', 1),
    (57, 'Silence is a source of Great Strength.', 'Lao Tzu', 1),
    (58, 'Doing nothing is better than being busy doing nothing.', 'Lao Tzu', 1),
    (59, 'The wise man is one who, knows, what he does not know.', 'Lao Tzu', 1),
    (60, 'Knowledge is a treasure, but practice is the key to it.', 'Lao Tzu', 1),
    (61, 'Great acts are made up of small deeds.', 'Lao Tzu', 1),
    (62, 'Do your work, then step back. The only path to serenity.', 'Lao Tzu', 1),
    (63, 'If you want to know me, look inside your heart.', 'Lao Tzu', 1),
    (64, 'Let it be still, and it will gradually become clear.', 'Lao Tzu', 1),
    (65, 'Take care with the end as you do with the beginning.', 'Lao Tzu', 1),
    (66, 'When you accept yourself, the whole world accepts you.', 'Lao Tzu', 1),
    (67, 'Hold faithfulness and sincerity as first principles.', 'Confucius', 1),
    (68, 'Roads were made for journeys not destinations.', 'Confucius', 1),
    (69, 'Settle one difficulty, and you keep a hundred away.', 'Confucius', 1),
    (70, 'One joy dispels a hundred cares.', 'Confucius', 1),
    (71, 'The Essence of Knowledge is, having it, to use it.', 'Confucius', 1),
    (72, 'Wherever you go, go with all your heart.', 'Confucius', 1),
    (73, 'Be silent or let thy words be worth more than silence.', 'Pythagoras', 1),
    (74, 'Silence is better than unmeaning words.', 'Pythagoras', 1),
    (75, 'Above all things, respect yourself.', 'Pythagoras', 1),
    (76, 'You should make great things, not promising great things.', 'Pythagoras', 1),
    (77, 'Luck is what happens when preparation meets opportunity.', 'Seneca', 1),
    (78, 'There is no easy way from the earth to the stars.', 'Seneca', 1),
    (79, 'Difficulties strengthen the mind, as labor does the body.', 'Seneca', 1),
    (80, 'He who is brave is free.', 'Seneca', 1),
    (81, 'Associate with people who are likely to improve you.', 'Seneca', 1),
    (82, 'Only time can heal what reason cannot.', 'Seneca', 1),
    (83, 'Most powerful is he who has himself in his own power.', 'Seneca', 1),
    (84, 'Wealth is the slave of a wise man. The master of a fool.', 'Seneca', 1),
    (85, 'As long as you live, keep learning how to live.', 'Seneca', 1),
    (86, 'Fate leads the willing and drags along the reluctant.', 'Seneca', 1),
    (87, 'Every new beginning comes from some other beginning''s end.', 'Seneca', 1),
    (88, 'We learn not in the school, but in life.', 'Seneca', 1),
    (89, 'Life is long if you know how to use it.', 'Seneca', 1),
    (90, 'To wish to be well is a part of becoming well.', 'Seneca', 1),
    (91, 'Nothing is more honorable than a grateful heart.', 'Seneca', 1),
    (92, 'While we wait for life, life passes.', 'Seneca', 1),
    (93, 'It is a rough road that leads to the heights of greatness.', 'Seneca', 1),
    (94, 'Whatever can happen at any time can happen today.', 'Seneca', 1),
    (95, 'It is quality rather than quantity that matters.', 'Seneca', 1),
    (96, 'The only true wisdom is in knowing you know nothing.', 'Socrates', 1),
    (97, 'To find yourself, think for yourself.', 'Socrates', 1),
    (98, 'Let him who would move the world first move himself.', 'Socrates', 1),
    (99, 'The only good is knowledge and the only evil is ignorance.', 'Socrates', 1),
    (100, 'We cannot live better than in seeking to become better.', 'Socrates', 1),
    (101, 'Be as you wish to seem.', 'Socrates', 1),
    (102, 'Those who are hardest to love need it the most.', 'Socrates', 1),
    (103, 'To move the world we must move ourselves.', 'Socrates', 1),
    (104, 'After all is said and done, more is said than done.', 'Aesop', 1),
    (105, 'It is easy to be brave at a safe distance.', 'Aesop', 1),
    (106, 'In trying to please all, he had pleased none.', 'Aesop', 1),
    (107, 'To seek is to suffer. To seek nothing is bliss.', 'Bodhidharma', 1),
    (108, 'Not engaging in ignorance is wisdom.', 'Bodhidharma', 1),
    (109, 'Freeing oneself from words is liberation.', 'Bodhidharma', 1),
    (110, 'All know the way of prosperity, only few walk it.', 'Bodhidharma', 1),
    (111, 'Reality has no inside, outside, or middle part.', 'Bodhidharma', 1),
    (112, 'The beginning is the most important part of the work.', 'Plato', 1),
    (113, 'A house that has a library in it has a soul.', 'Plato', 1),
    (114, 'The greatest wealth is to live content with little.', 'Plato', 1),
    (115, 'If you look for perfection, you''ll never be content.', 'Leo Tolstoy', 1),
    (116, 'If you want to be happy, be.', 'Leo Tolstoy', 1),
    (117, 'The two most powerful warriors are patience and time.', 'Leo Tolstoy', 1),
    (118, 'True life is lived when tiny changes occur.', 'Leo Tolstoy', 1),
    (119, 'The sun shines not on us but in us.', 'John Muir', 1),
    (120, 'The power of imagination makes us infinite.', 'John Muir', 1),
    (121, 'One touch of nature makes the whole world kin.', 'John Muir', 1);

-- Fill quote table with Polish quotes
INSERT OR REPLACE INTO quote_table (quote_id, quote, author, quote_lang) VALUES
    (122, 'Najprostsze rzeczy są często najprawdziwsze.', 'Richard Bach', 2),
    (123, 'Zwolnij, upraszczaj, bądź życzliwy.', 'Naomi Judd', 2),
    (124, 'Idę więc do lasu, by stracić rozum i odnaleźć duszę.', 'John Muir', 2),
    (125, 'Zawsze to rzeczy proste są źródłem wspaniałych.', 'Amelia Barr', 2),
    (126, 'Pamiętaj jednak, że teraźniejszość jest największym darem.', 'Dan Santat', 2),
    (127, 'Rana jest miejscem, przez które wpada do ciebie Światło.', 'Rumi', 2),
    (128, 'Daj przyciągać się temu, co prawdziwie kochasz.', 'Rumi', 2),
    (129, 'A ty? Kiedy rozpoczniesz długą podróż w głąb siebie?', 'Rumi', 2),
    (130, 'Gdziekolwiek jesteś, cokolwiek robisz - kochaj.', 'Rumi', 2),
    (131, 'Rozsądek jest bezsilny w wyrazie Miłości.', 'Rumi', 2),
    (132, 'Jedynie z sercem możesz dotknąć nieba.', 'Rumi', 2),
    (133, 'Nie czuj się samotny, w tobie mieszka cały wszechświat.', 'Rumi', 2),
    (134, 'Lśnij, jak gdyby cały świat należał do ciebie.', 'Rumi', 2),
    (135, 'Ważne jest, jak szybko robisz to, co podpowiada ci dusza.', 'Rumi', 2),
    (136, 'Niech cisza zabierze cię do sedna życia.', 'Rumi', 2),
    (137, 'Jedynym nieprzemijającym pięknem jest piękno serca.', 'Rumi', 2),
    (138, 'Spróbuj czegoś nowego. Poddaj się.', 'Rumi', 2),
    (139, 'Pokój pochodzi z wnętrza. Nie szukaj go na zewnątrz.', 'Budda', 2),
    (140, 'Kwestionuj wszystko. Znajdź własne światło.', 'Budda', 2),
    (141, 'Droga do szczęścia nie istnieje: to szczęście jest drogą.', 'Budda', 2),
    (142, 'Nie szukaj sanktuarium u nikogo poza sobą.', 'Budda', 2),
    (143, 'Zrozumienie rodzi prawdziwą miłość.', 'Budda', 2),
    (144, 'Szczęście przychodzi, gdy twoje czyny i słowa służą innym.', 'Budda', 2),
    (145, 'Najwspanialszą modlitwą jest cierpliwość.', 'Budda', 2),
    (146, 'Twoje ciernie rzucane w moim milczeniu stają się kwiatami.', 'Budda', 2),
    (147, 'Noś swe ego niczym luźną odzież.', 'Budda', 2),
    (148, 'Zdolny jest ten, który ma się za zdolnego.', 'Budda', 2),
    (149, 'Złość nie zniknie tak długo, jak w umyśle będzie żal.', 'Budda', 2),
    (150, 'Bądź tam, gdzie jesteś; inaczej przegapisz swoje życie.', 'Budda', 2),
    (151, 'Bądź czujny; chroń swój umysł przed negatywnymi myślami.', 'Budda', 2),
    (152, 'Bądź świadomy teraźniejszości.', 'Budda', 2),
    (153, 'Umysł jest wszystkim. Stajesz się tym, co myślisz.', 'Budda', 2),
    (154, 'Lekcja jest prosta. Postępuj słusznie. Bądź bez skazy.', 'Budda', 2),
    (155, 'Dzbanek wypełnia się kropla po kropli.', 'Budda', 2),
    (156, 'Wypracuj własne zbawienie. Nie polegaj na innych.', 'Budda', 2),
    (157, 'Zadowolenie jest największym bogactwem.', 'Budda', 2),
    (158, 'Gdy umysł jest czysty, radość podąża za tobą niczym cień.', 'Budda', 2),
    (159, 'Im mniej masz rzeczy, tym mniej masz zmartwień.', 'Budda', 2),
    (160, 'Nie możesz pojąć piękna kwiatu, wyrywając jego płatki.', 'Budda', 2),
    (161, 'Bądź swoją własną lampą.', 'Budda', 2),
    (162, 'Cierpliwość jest najwyższą formą ascetyzmu.', 'Budda', 2),
    (163, 'Nade wszystko, nie trać chęci do kroczenia.', 'Budda', 2),
    (164, 'Każdy człowiek jest sprawcą swego zdrowia lub choroby.', 'Budda', 2),
    (165, 'Mądrość wzrasta ściśle wraz ze spadkiem gniewu.', 'Budda', 2),
    (166, 'Czy jest w porę? Czy jest niezbędne? Czy jest dobre?', 'Budda', 2),
    (167, 'To, co najprostsze jest często najprawdziwsze.', 'Richard Bach', 2),
    (168, 'Nie ma wielkości, gdzie nie ma prostoty, dobroci i prawdy.', 'Lew Tołstoj', 2),
    (169, 'Nawet najciemniejsza noc się skończy i wzejdzie słońce.', 'Victor Hugo', 2),
    (170, 'Śmiech to słońce, spędza zimę z twarzy ludzkiej.', 'Victor Hugo', 2),
    (171, 'Łatwo jest szukać błędów, trudniej jest czynić lepiej.', 'Plutarch', 2),
    (172, 'Im większa przeszkoda, tym więcej chwały w jej pokonaniu.', 'Molière', 2),
    (173, 'Połowy dokonał, kto zaczął. Odważ się być mądrym. Zacznij.', 'Horacy', 2),
    (174, 'Ciekawość jest początkiem mądrości.', 'Sokrates', 2),
    (175, 'Podróż tysiąca mil zaczyna się od jednego kroku.', 'Lao Tzu', 2),
    (176, 'Przygoda warta jest każdego trudu.', 'Arystoteles', 2),
    (177, 'Jest czas na słowa, jest też czas na sen.', 'Homer', 2),
    (178, 'Słońce odnawia się co dzień.', 'Heraklit', 2),
    (179, 'Dusza nie myśli bez obrazu.', 'Arystoteles', 2),
    (180, 'Wybierz silną duszę zamiast silnego ciała.', 'Pitagoras', 2),
    (181, 'Cokolwiek zadowala duszę, jest prawdą.', 'Walt Whitman', 2),
    (182, 'Cały świat słyszy muzykę duszy.', 'Lao Tzu', 2),
    (183, 'Mimo, że natura się nie spieszy, i tak wszystko realizuje.', 'Lao Tzu', 2),
    (184, 'Cisza jest źródłem Wielkiej Siły.', 'Lao Tzu', 2),
    (185, 'Mędrcem jest ten, który wie, czego nie wie.', 'Lao Tzu', 2),
    (186, 'Wiedza jest skarbem, a praktyka kluczem.', 'Lao Tzu', 2),
    (187, 'Wielkie czyny składają się z drobnych uczynków.', 'Lao Tzu', 2),
    (188, 'Jeśli chcesz mnie poznać, zajrzyj w głąb swego serca.', 'Lao Tzu', 2),
    (189, 'Pozostaw w spokoju to, co chcesz ujrzeć przejrzystym.', 'Lao Tzu', 2),
    (190, 'Dbaj o koniec tak, jak dbasz o początek.', 'Lao Tzu', 2),
    (191, 'Gdy zaakceptujesz siebie, cały świat zaakceptuje ciebie.', 'Lao Tzu', 2),
    (192, 'Ten, który chce przenosić górę, zaczyna od małych kamieni.', 'Konfucjusz', 2),
    (193, 'Rozwiąż jeden problem, a unikniesz setki innych.', 'Konfucjusz', 2),
    (194, 'Jedna uciecha rozpędza tysiące smutków.', 'Konfucjusz', 2),
    (195, 'Istotą Wiedzy jest jej posiadanie, a nie wykorzystanie.', 'Konfucjusz', 2),
    (196, 'Gdziekolwiek zmierzasz, rób to pełnią serca.', 'Konfucjusz', 2),
    (197, 'Milczenie jest lepsze niż słowa nic nieznaczące.', 'Pitagoras', 2),
    (198, 'Przede wszystkim szanuj siebie.', 'Pitagoras', 2),
    (199, 'Nad chmurą rzucającą cień jest gwiazda rzucająca światło.', 'Pitagoras', 2),
    (200, 'Rób wielkie rzeczy, nie obiecując wielkich rzeczy.', 'Pitagoras', 2),
    (201, 'Trudno jest kroczyć wieloma ścieżkami życia jednocześnie.', 'Pitagoras', 2),
    (202, 'Szczęście jest tam, gdzie przygotowanie spotyka okazję.', 'Seneka', 2),
    (203, 'Droga do gwiazd nie jest łatwa z ziemi.', 'Seneka', 2),
    (204, 'Trudności wzmacniają umysł tak, jak praca wzmacnia ciało.', 'Seneka', 2),
    (205, 'Ten kto jest odważny, jest wolny.', 'Seneka', 2),
    (206, 'Przystawaj z ludźmi, którzy mogą pomóc ci się rozwijać.', 'Seneka', 2),
    (207, 'Czas leczy to, czego rozum nie jest w stanie.', 'Seneka', 2),
    (208, 'Najpotężniejszy jest ten, kto panuje nad sobą.', 'Seneka', 2),
    (209, 'Bogactwa mądremu służą, nad głupim panują.', 'Seneka', 2),
    (210, 'Dopóki żyjesz, ucz się jak żyć.', 'Seneka', 2),
    (211, 'Los prowadzi ochoczych i ciągnie za sobą opornych.', 'Seneka', 2),
    (212, 'Każdy nowy początek wynika z końca innego początku.', 'Seneka', 2),
    (213, 'Nie uczymy się w szkole, lecz w trakcie życia.', 'Seneka', 2),
    (214, 'Życie jest długie, jeśli wiesz jak z niego korzystać.', 'Seneka', 2),
    (215, 'Pragnienie pomyślności jest częścią drogi do pomyślności.', 'Seneka', 2),
    (216, 'Nie ma nic bardziej zaszczytnego niż wdzięczne serce.', 'Seneka', 2),
    (217, 'Życie przemija, podczas gdy na nie czekamy.', 'Seneka', 2),
    (218, 'Nie ważne ile masz książek, liczy się tylko ich jakość.', 'Seneka', 2),
    (219, 'Wyboista jest droga prowadząca do wielkości.', 'Seneka', 2),
    (220, 'Liczy się jakość, nie ilość.', 'Seneka', 2),
    (221, 'Aby odnaleźć siebie, należy myśleć samodzielnie.', 'Sokrates', 2),
    (222, 'Jedynym dobrem jest wiedza, a jedynym złem niewiedza.', 'Sokrates', 2),
    (223, 'Bądź osobą, którą chcesz, aby ludzie myśleli, że jesteś.', 'Sokrates', 2),
    (224, 'By zmienić świat, musimy zmienić siebie.', 'Sokrates', 2),
    (225, 'Zdradzając przyjaciela, często pogrążasz też siebie.', 'Ezop', 2),
    (226, 'Odważnym jest być łatwo z bezpiecznej odległości.', 'Ezop', 2),
    (227, 'Próbując zadowolić każdego, nie zadowolił nikogo.', 'Ezop', 2),
    (228, 'Pragnienie jest cierpieniem. Brak pragnień jest rozkoszą.', 'Bodhidharma', 2),
    (229, 'Unikanie niewiedzy jest mądrością.', 'Bodhidharma', 2),
    (230, 'Uwolnienie się od słów jest wolnością.', 'Bodhidharma', 2),
    (231, 'Ścieżka jest zasadniczo idealna. Nie wymaga udoskonalenia.', 'Bodhidharma', 2),
    (232, 'Każdy zna drogę do pomyślności, tylko niewielu nią kroczy.', 'Bodhidharma', 2),
    (233, 'Początek jest najważniejszą częścią zadania.', 'Platon', 2),
    (234, 'Dom z biblioteczką to dom z duszą.', 'Platon', 2),
    (235, 'Jeśli chcesz być szczęśliwy, po prostu bądź.', 'Lew Tołstoj', 2),
    (236, 'Dwóch najpotężniejszych wojowników to czas i cierpliwość.', 'Lew Tołstoj', 2),
    (237, 'Słońce nie świeci na nas, lecz w nas.', 'John Muir', 2),
    (238, 'Moc wyobraźni czyni nas nieskończonymi.', 'John Muir', 2),
    (239, 'Jeden dotyk natury łączy cały świat.', 'John Muir', 2);

-- Fill quote table with French quotes
INSERT OR REPLACE INTO quote_table (quote_id, quote, author, quote_lang) VALUES
    (240, 'Les choses les plus simples sont souvent les plus vraies.', 'Richard Bach', 3),
    (241, 'Ralentissez, simplifiez et soyez gentil.', 'Naomi Judd', 3),
    (242, 'La blessure est l''endroit où la Lumière entre en vous.', 'Rumi', 3),
    (243, 'Là où il y a des ruines, il y a l''espoir d''un trésor.', 'Rumi', 3),
    (244, 'La raison est impuissante dans l''expression de l''Amour.', 'Rumi', 3),
    (245, 'Ne vous sentez pas seul, l''univers entier est en vous.', 'Rumi', 3),
    (246, 'Brillez comme si l''univers entier était à vous.', 'Rumi', 3),
    (247, 'Essayez quelque chose de différent. Abandonnez-vous.', 'Rumi', 3),
    (248, 'Doutez de tout. Trouvez votre propre lumière.', 'Bouddha', 3),
    (249, 'Ne cherchez pas de sanctuaire en dehors de vous-même.', 'Bouddha', 3),
    (250, 'Le véritable amour naît de la compréhension.', 'Bouddha', 3),
    (251, 'La plus grande prière est la patience.', 'Bouddha', 3),
    (252, 'Portez votre ego comme un vêtement ample.', 'Bouddha', 3),
    (253, 'Celui qui se croit capable est capable.', 'Bouddha', 3),
    (254, 'Soyez là où vous êtes, sinon vous raterez votre vie.', 'Bouddha', 3),
    (255, 'Soyez très conscient du présent.', 'Bouddha', 3),
    (256, 'L''esprit est tout. Ce que vous pensez, vous le devenez.', 'Bouddha', 3),
    (257, 'Une cruche se remplit goutte à goutte.', 'Bouddha', 3),
    (258, 'Le contentement est la plus grande richesse.', 'Bouddha', 3),
    (259, 'Moins vous avez, moins vous avez à vous inquiéter.', 'Bouddha', 3),
    (260, 'Soyez votre propre lampe.', 'Bouddha', 3),
    (261, 'La patience est la plus haute ascèse.', 'Bouddha', 3),
    (262, 'Vivez chaque acte pleinement, comme si c''était le dernier.', 'Bouddha', 3),
    (263, 'Surtout, ne perdez pas votre envie de marcher.', 'Bouddha', 3),
    (264, 'Est-il opportun ? Est-il nécessaire ? Est-il bon ?', 'Bouddha', 3),
    (265, 'L''émerveillement est le début de la sagesse.', 'Socrate', 3),
    (266, 'Un voyage de mille kilomètres commence par un seul pas.', 'Lao Tseu', 3),
    (267, 'L''aventure en vaut la peine.', 'Aristote', 3),
    (268, 'Le soleil est nouveau chaque jour.', 'Heraclitus', 3),
    (269, 'Soyez heureux pour ce moment. Ce moment est votre vie.', 'Omar Khayyam', 3),
    (270, 'L''âme ne pense jamais sans une image.', 'Aristote', 3),
    (271, 'Choisissez plutôt d''être fort d''âme que fort de corps.', 'Pythagore', 3),
    (272, 'Un véritable ami est une âme dans deux corps.', 'Aristote', 3),
    (273, 'Tout ce qui satisfait l''âme est la vérité.', 'Walt Whitman', 3),
    (274, 'La musique dans l''âme peut être entendue par l''univers.', 'Lao Tseu', 3),
    (275, 'La nature ne se presse pas, et pourtant tout est accompli.', 'Lao Tseu', 3),
    (276, 'Le silence est une source de grande force.', 'Lao Tseu', 3),
    (277, 'Ne rien faire est mieux que d''être occupé à ne rien faire.', 'Lao Tseu', 3),
    (278, 'L''homme sage est celui qui sait ce qu''il ne sait pas.', 'Lao Tseu', 3),
    (279, 'Les grands actes sont faits de petites actions.', 'Lao Tseu', 3),
    (280, 'Prenez soin de la fin comme vous le faites avec le début.', 'Lao Tseu', 3),
    (281, 'Réglez une difficulté, et vous en éloignez une centaine.', 'Confucius', 3),
    (282, 'Une joie dissipe cent soucis.', 'Confucius', 3),
    (283, 'Le silence vaut mieux que des mots dénués de sens.', 'Pythagore', 3),
    (284, 'Par-dessus tout, respectez-vous.', 'Pythagore', 3),
    (285, 'Il n''y a pas de chemin facile de la terre aux étoiles.', 'Sénèque', 3),
    (286, 'Celui qui est courageux est libre.', 'Sénèque', 3),
    (287, 'Seul le temps peut guérir ce que la raison ne peut pas.', 'Sénèque', 3),
    (288, 'Le plus puissant est celui qui se tient en son pouvoir.', 'Sénèque', 3),
    (289, 'Le destin mène les volontaires et entraîne les réticents.', 'Sénèque', 3),
    (290, 'Nous n''apprenons pas à l''école, mais dans la vie.', 'Sénèque', 3),
    (291, 'La vie est longue si vous savez comment l''utiliser.', 'Sénèque', 3),
    (292, 'Souhaiter être bien fait partie du fait de devenir bien.', 'Sénèque', 3),
    (293, 'Pendant que nous attendons la vie, la vie passe.', 'Sénèque', 3),
    (294, 'C''est la qualité plutôt que la quantité qui compte.', 'Sénèque', 3),
    (295, 'Pour vous trouver, pensez par vous-même.', 'Socrate', 3),
    (296, 'On ne peut vivre mieux qu''en cherchant à devenir meilleur.', 'Socrate', 3),
    (297, 'Soyez tel que vous souhaitez paraître.', 'Socrate', 3),
    (298, 'Après tout, il y a plus à dire qu''à faire.', 'Ésope', 3),
    (299, 'Il est facile d''être courageux à une distance sûre.', 'Ésope', 3),
    (300, 'En essayant de plaire à tous, il n''a plu à personne.', 'Ésope', 3),
    (301, 'Ne pas s''engager dans l''ignorance est la sagesse.', 'Bodhidharma', 3),
    (302, 'Se libérer des mots est la libération.', 'Bodhidharma', 3),
    (303, 'Le début est la partie la plus importante du travail.', 'Platon', 3),
    (304, 'La plus grande richesse est de se contenter de peu.', 'Platon', 3),
    (305, 'Si vous voulez être heureux, faites-le.', 'Léon Tolstoï', 3),
    (306, 'Le soleil ne brille pas sur nous mais en nous.', 'John Muir', 3),
    (307, 'Le pouvoir de l''imagination nous rend infinis.', 'John Muir', 3),
    (308, 'Une touche de nature rend le monde entier aimable.', 'John Muir', 3);

-- Fill quote table with Spanish quotes
INSERT OR REPLACE INTO quote_table (quote_id, quote, author, quote_lang) VALUES
    (309, 'Las cosas más sencillas son a menudo las más auténticas.', 'Richard Bach', 4),
    (310, 'Cálmate, simplifica y sé amable.', 'Naomi Judd', 4),
    (311, 'Y me voy al bosque a perder la cabeza y encontrar el alma.', 'John Muir', 4),
    (312, 'Lo sencillo es siempre lo que produce lo maravilloso.', 'Amelia Barr', 4),
    (313, 'Pero recuerda: no hay mayor regalo que el presente.', 'Dan Santat', 4),
    (314, 'La herida es el lugar por donde entra la luz.', 'Rumi', 4),
    (315, 'Naciste con alas. ¿Por qué prefieres gatear por la vida?', 'Rumi', 4),
    (316, 'Todo lo del universo está dentro de ti. Pregúntatelo todo.', 'Rumi', 4),
    (317, 'Donde hay restos, hay esperanza para encontrar un tesoro.', 'Rumi', 4),
    (318, 'Estés donde estés y hagas lo que hagas, enamórate.', 'Rumi', 4),
    (319, 'La razón se vuelve impotente cuando se expresa el amor.', 'Rumi', 4),
    (320, 'Solo con el corazón puedes tocar el cielo.', 'Rumi', 4),
    (321, 'No te sientas solo, todo el universo está dentro de ti.', 'Rumi', 4),
    (322, 'Brilla como si todo el universo fuese tuyo.', 'Rumi', 4),
    (323, 'Deja que el silencio te lleve a la esencia de la vida.', 'Rumi', 4),
    (324, 'La única belleza duradera es la belleza del corazón.', 'Rumi', 4),
    (325, 'Prueba algo diferente. Ríndete.', 'Rumi', 4),
    (326, 'La paz está dentro de ti. No la busques fuera.', 'Buda', 4),
    (327, 'Duda de todo. Encuentra tu luz.', 'Buda', 4),
    (328, 'No busques un santuario en alguien que no seas tú.', 'Buda', 4),
    (329, 'Nada existe nunca por sí solo. Todo está relacionado.', 'Buda', 4),
    (330, 'El amor verdadero nace de la comprensión.', 'Buda', 4),
    (331, 'La mejor oración es la paciencia.', 'Buda', 4),
    (332, 'Lleva tu ego como un atuendo muy holgado.', 'Buda', 4),
    (333, 'Es capaz aquel que cree que es capaz.', 'Buda', 4),
    (334, 'Recordar un error es como sostener una carga en la mente.', 'Buda', 4),
    (335, 'Sé consciente en gran medida del presente.', 'Buda', 4),
    (336, 'La mente lo es todo. Te conviertes en lo que piensas.', 'Buda', 4),
    (337, 'La lección es sencilla: haz lo correcto. Sé puro.', 'Buda', 4),
    (338, 'Una jarra se llena gota a gota.', 'Buda', 4),
    (339, 'Soluciona tu salvación. No dependas de nadie.', 'Buda', 4),
    (340, 'La alegría es la mayor riqueza.', 'Buda', 4),
    (341, 'Sé tu propia luz.', 'Buda', 4),
    (342, 'La paciencia es el mayor ascetismo.', 'Buda', 4),
    (343, 'Vive cada acción al máximo, como si fuera la última.', 'Buda', 4),
    (344, 'Ante todo, no pierdas las ganas de caminar.', 'Buda', 4),
    (345, 'Cada ser humano es autor de su propia salud o enfermedad.', 'Buda', 4),
    (346, '¿Es oportuno? ¿Es necesario? ¿Es bueno?', 'Buda', 4),
    (347, 'Nada es suficiente para el que lo suficiente es poco.', 'Epicuro', 4),
    (348, 'Incluso la noche más oscura terminará y el sol saldrá.', 'Victor Hugo', 4),
    (349, 'Equilibrarlo todo es bueno, pero con armonía es mejor.', 'Victor Hugo', 4),
    (350, 'Criticar es fácil; hacerlo mejor puede ser más difícil.', 'Plutarco', 4),
    (351, 'Cuanto mayor es el obstáculo, mayor gloria es superarlo.', 'Molière', 4),
    (352, 'Hacerse preguntas es el principio de la sabiduría.', 'Sócrates', 4),
    (353, 'Un viaje de mil kilómetros comienza con un simple paso.', 'Lao Tzu', 4),
    (354, 'La aventura vale la pena.', 'Aristóteles', 4),
    (355, 'El sol es nuevo cada día.', 'Heráclito', 4),
    (356, 'Sé feliz en este momento. Este momento es tu vida.', 'Omar Jayam', 4),
    (357, 'El alma nunca piensa sin tener una imagen.', 'Aristóteles', 4),
    (358, 'Un amigo de verdad es un alma en dos cuerpos.', 'Aristóteles', 4),
    (359, 'Lo que satisface el alma es la verdad.', 'Walt Whitman', 4),
    (360, 'La música del alma se oye en todo el universo.', 'Lao Tzu', 4),
    (361, 'La naturaleza no se apresura. Sin embargo, todo se logra.', 'Lao Tzu', 4),
    (362, 'El silencio es una fuente de gran fuerza.', 'Lao Tzu', 4),
    (363, 'Es mejor no hacer nada que estar ocupado sin hacer nada.', 'Lao Tzu', 4),
    (364, 'Un hombre sabio es aquel que sabe que no sabe.', 'Lao Tzu', 4),
    (365, 'Las grandes acciones están hechas de pequeños actos.', 'Lao Tzu', 4),
    (366, 'Si quieres conocerme, mira dentro de tu corazón.', 'Lao Tzu', 4),
    (367, 'El que sabe lo que es necesario siempre tendrá suficiente.', 'Lao Tzu', 4),
    (368, 'Déjalo quieto y poco a poco se aclarará.', 'Lao Tzu', 4),
    (369, 'Cuida el final como hiciste con el principio.', 'Lao Tzu', 4),
    (370, 'Cuando te aceptes, todo el mundo te aceptará.', 'Lao Tzu', 4),
    (371, 'No importa lo despacio que vayas mientras no te detengas.', 'Confucio', 4),
    (372, 'El que mueve una montaña empieza por llevarse piedrecitas.', 'Confucio', 4),
    (373, 'Ten la lealtad y la honestidad como principios básicos.', 'Confucio', 4),
    (374, 'Resuelve un problema y alejarás cien.', 'Confucio', 4),
    (375, 'La esencia del conocimiento es tenerlo para usarlo.', 'Confucio', 4),
    (376, 'Vayas a donde vayas, ve con todas tus fuerzas.', 'Confucio', 4),
    (377, 'El silencio es mejor que las palabras absurdas.', 'Pitágoras', 4),
    (378, 'Por encima de todo, respétate.', 'Pitágoras', 4),
    (379, 'Deberías hacer grandes cosas, no prometer grandes cosas.', 'Pitágoras', 4),
    (380, 'Aquel que es valiente es libre.', 'Séneca', 4),
    (381, 'Rodéate de personas que probablemente te hagan mejorar.', 'Séneca', 4),
    (382, 'Solo el tiempo cura lo que la razón no puede.', 'Séneca', 4),
    (383, 'Lo más poderoso que tiene un hombre es su propia fuerza.', 'Séneca', 4),
    (384, 'La riqueza es esclava del sabio y dueña del necio.', 'Séneca', 4),
    (385, 'Cada nuevo comienzo viene del final de otro comienzo.', 'Séneca', 4),
    (386, 'No se aprende en la escuela, sino en la vida.', 'Séneca', 4),
    (387, 'La vida es larga si sabes cómo aprovecharla.', 'Séneca', 4),
    (388, 'Desear estar bien forma parte de lograr estar bien.', 'Séneca', 4),
    (389, 'No hay nada más honorable que un corazón agradecido.', 'Séneca', 4),
    (390, 'Mientras esperamos vivir, la vida pasa.', 'Séneca', 4),
    (391, 'Lo que puede pasar en cualquier momento puede pasar hoy.', 'Séneca', 4),
    (392, 'Lo importante es la calidad, no la cantidad.', 'Séneca', 4),
    (393, 'La única y verdadera sabiduría es saber que no sabes nada.', 'Sócrates', 4),
    (394, 'Para encontrarte a ti mismo, piensa por ti mismo.', 'Sócrates', 4),
    (395, 'Deja primero que cambie aquel que quiera cambiar el mundo.', 'Sócrates', 4),
    (396, 'No podemos vivir mejor que intentando ser mejores.', 'Sócrates', 4),
    (397, 'Sé aquello que deseas parecer.', 'Sócrates', 4),
    (398, 'Para cambiar el mundo tenemos que cambiar nosotros mismos.', 'Sócrates', 4),
    (399, 'Entre lo dicho y lo hecho hay más de dicho que de hecho.', 'Esopo', 4),
    (400, 'Es fácil ser valiente desde una distancia segura.', 'Esopo', 4),
    (401, 'Los que veneran no saben y los que saben no veneran.', 'Bodhidharma', 4),
    (402, 'Buscar es sufrir. No buscar nada es la felicidad.', 'Bodhidharma', 4),
    (403, 'No participar en la ignorancia refleja sabiduría.', 'Bodhidharma', 4),
    (404, 'Liberarse uno mismo de las palabras es emancipador.', 'Bodhidharma', 4),
    (405, 'La realidad no tiene parte interior, exterior ni central.', 'Bodhidharma', 4),
    (406, 'El principio es la parte más importante de una tarea.', 'Platón', 4),
    (407, 'Un hogar que tiene una biblioteca es un hogar con alma.', 'Platón', 4),
    (408, 'La mayor riqueza es vivir feliz con poco.', 'Platón', 4),
    (409, 'Si buscas la perfección, nunca estarás satisfecho.', 'León Tolstói', 4),
    (410, 'Si quieres ser feliz, selo.', 'León Tolstói', 4),
    (411, 'Todo lo que comprendo lo entiendo porque amo.', 'León Tolstói', 4),
    (412, 'El sol no brilla sobre nosotros, sino en nosotros.', 'John Muir', 4),
    (413, 'El poder de la imaginación nos vuelve infinitos.', 'John Muir', 4);

-- Fill quote table with German quotes
INSERT OR REPLACE INTO quote_table (quote_id, quote, author, quote_lang) VALUES
    (414, 'Die einfachsten Dinge sind oft die wahrsten.', 'Richard Bach', 5),
    (415, 'Werde langsamer, vereinfache und sei freundlich.', 'Naomi Judd', 5),
    (416, 'Es ist immer nur das Einfache, das Großartiges bewirkt.', 'Amelia Barr', 5),
    (417, 'Wo Ruinen sind, besteht Hoffnung auf einen Schatz.', 'Rumi', 5),
    (418, 'Wo immer du bist und was auch immer du tust, sei verliebt.', 'Rumi', 5),
    (419, 'Die Vernunft ist machtlos im Ausdruck der Liebe.', 'Rumi', 5),
    (420, 'Nur aus dem Herzen kannst du den Himmel berühren. ', 'Rumi', 5),
    (421, 'Fühle dich nicht einsam, das ganze Universum ist in dir.', 'Rumi', 5),
    (422, 'Leuchte, als würde das ganze Universum dir gehören.', 'Rumi', 5),
    (423, 'Lass die Stille dich zum Kern des Lebens bringen.', 'Rumi', 5),
    (424, 'Probiere etwas anderes. Gib auf.', 'Rumi', 5),
    (425, 'Frieden kommt von innen. Suche ihn nicht draußen.', 'Buddha', 5),
    (426, 'Zweifle an allem. Finde dein eigenes Licht.', 'Buddha', 5),
    (427, 'Es gibt keinen Weg zum Glück. Glücklichsein ist der Weg.', 'Buddha', 5),
    (428, 'Wahre Liebe wird aus Verständnis geboren.', 'Buddha', 5),
    (429, 'Das beste Gebet ist Geduld.', 'Buddha', 5),
    (430, 'Trage dein Ego wie ein locker anliegendes Kleidungsstück.', 'Buddha', 5),
    (431, 'Der ist fähig, der denkt, dass er fähig ist.', 'Buddha', 5),
    (432, 'Sei, wo du bist; sonst wirst du dein Leben verpassen.', 'Buddha', 5),
    (433, 'Sei dir der Gegenwart sehr bewusst.', 'Buddha', 5),
    (434, 'Der Verstand ist alles. Was wir denken, das werden wir.', 'Buddha', 5),
    (435, 'Die Lehre ist einfach. Tu das Richtige. Sei rein.', 'Buddha', 5),
    (436, 'Ein Krug füllt sich Tropfen für Tropfen.', 'Buddha', 5),
    (437, 'Zufriedenheit ist der größte Reichtum.', 'Buddha', 5),
    (438, 'Seid eine Lampe für euch selbst.', 'Buddha', 5),
    (439, 'Geduld ist die höchste Askese.', 'Buddha', 5),
    (440, 'Lebe jede Handlung voll aus, als wäre es deine letzte.', 'Buddha', 5),
    (441, 'Verliere vor allem nicht dein Verlangen zu gehen.', 'Buddha', 5),
    (442, 'Wem genug zu wenig ist, dem ist nichts genug.', 'Epikur', 5),
    (443, 'Weisheit beginnt im Staunen.', 'Sokrates', 5),
    (444, 'Abenteuer sind erstrebenswert.', 'Aristoteles', 5),
    (445, 'Die Sonne ist neu an jedem Tag.', 'Heraklit', 5),
    (446, 'Die Seele denkt nie ohne ein Bild.', 'Aristoteles', 5),
    (447, 'Freundschaft ist eine Seele in zwei Körpern.', 'Aristoteles', 5),
    (448, 'Was die Seele befriedigt, ist Wahrheit.', 'Walt Whitman', 5),
    (449, 'Musik in der Seele ist vom Universum zu hören.', 'Lao Tzu', 5),
    (450, 'Die Natur eilt nicht und dennoch wird alles erreicht. ', 'Lao Tzu', 5),
    (451, 'Stille ist eine Quelle großer Stärke.', 'Lao Tzu', 5),
    (452, 'Nichtstun ist besser, als mit viel Mühe nichts schaffen. ', 'Lao Tzu', 5),
    (453, 'Der weise Mann weiß, was er nicht weiß.', 'Lao Tzu', 5),
    (454, 'Wissen ist ein Schatz, aber Praxis ist der Schlüssel dazu.', 'Lao Tzu', 5),
    (455, 'Große Handlungen bestehen aus kleinen Taten.', 'Lao Tzu', 5),
    (456, 'Wenn du mich kennenlernen willst, schau in dein Herz.', 'Lao Tzu', 5),
    (457, 'Wer weiß, dass genug genug ist, wird immer genug haben.', 'Lao Tzu', 5),
    (458, 'Lass es still sein, es wird nach und nach klar werden.', 'Lao Tzu', 5),
    (459, 'Achte auf das Ende, wie du es mit dem Anfang tust.', 'Lao Tzu', 5),
    (460, 'Weisheit beginnt damit, die Dinge beim Namen zu nennen.', 'Konfuzius', 5),
    (461, 'Straßen wurden für Reisen gemacht, nicht Ziele.', 'Konfuzius', 5),
    (462, 'Eine Freude vertreibt hundert Sorgen.', 'Konfuzius', 5),
    (463, 'Die Essenz des Wissens ist, es zu haben, es zu benutzen.', 'Konfuzius', 5),
    (464, 'Wohin du auch gehst, geh mit ganzem Herzen.', 'Konfuzius', 5),
    (465, 'Sei still oder lass deine Worte mehr wert sein als Stille.', 'Pythagoras', 5),
    (466, 'Respektiere vor allem dich selbst.', 'Pythagoras', 5),
    (467, 'Du solltest Großes vollbringen, nicht Großes versprechen.', 'Pythagoras', 5),
    (468, 'Es gibt keinen einfachen Weg von der Erde zu den Sternen.', 'Seneca', 5),
    (469, 'Wer mutig ist, ist frei.', 'Seneca', 5),
    (470, 'Was der Verstand nicht heilen kann, heilt oft die Zeit.', 'Seneca', 5),
    (471, 'Der sei der Mächtigste, der sich selbst in der Gewalt hat.', 'Seneca', 5),
    (472, 'Solange du lebst, lerne weiter, wie man lebt.', 'Seneca', 5),
    (473, 'Jeder Neuanfang kommt vom Ende eines anderen Anfangs.', 'Seneca', 5),
    (474, 'Nicht für die Schule, sondern für das Leben lernen wir.', 'Seneca', 5),
    (475, 'Nichts ist ehrenwerter als ein dankbares Herz.', 'Seneca', 5),
    (476, 'Während wir auf das Leben warten, vergeht das Leben.', 'Seneca', 5),
    (477, 'Was jederzeit passieren kann, kann heute passieren.', 'Seneca', 5),
    (478, 'Es kommt eher auf Qualität als auf Quantität an.', 'Seneca', 5),
    (479, 'Um dich selbst zu finden, denke für dich selbst.', 'Sokrates', 5),
    (480, 'Sei, was du scheinen willst.', 'Sokrates', 5),
    (481, 'Um die Welt zu bewegen, müssen wir uns selbst bewegen.', 'Sokrates', 5),
    (482, 'Sich von Worten zu befreien ist Befreiung.', 'Bodhidharma', 5),
    (483, 'Der Anfang ist der wichtigste Teil der Arbeit.', 'Platon', 5),
    (484, 'Ein Haus mit einer Bücherei darin besitzt eine Seele.', 'Platon', 5),
    (485, 'Wer nach Perfektion strebt, wird niemals glücklich sein.', 'Lew Tolstoi', 5),
    (486, 'Willst du glücklich sein im Leben, dann sei es!', 'Lew Tolstoi', 5),
    (487, 'Die zwei mächtigsten Krieger sind Geduld und Zeit.', 'Lew Tolstoi', 5),
    (488, 'Winzige Veränderungen machen das wahre Leben aus.', 'Lew Tolstoi', 5),
    (489, 'Der klarste Weg ins Universum ist durch eine Waldwildnis.', 'John Muir', 5),
    (490, 'Die Sonne scheint nicht auf uns, sondern in uns.', 'John Muir', 5),
    (491, 'Die Macht der Vorstellungskraft macht uns grenzenlos.', 'John Muir', 5),
    (492, 'Eine Berührung von Natur macht die ganze Welt verwandt.', 'John Muir', 5);
