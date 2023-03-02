
# 10. HM / Generalization

## Мотивация

Хотим написать реализацию контейнера `Vec`:

```etude
    type Vec a = struct {
        data: *a,
        size: Int,
        capacity: Int,
    };
```

Также хотим иметь функции:

```
    of *Vec(a) -> a -> ()
    fun append vector value;
```

```
    of Int -> *Vec(a)
    fun make_vec intial_size;
```

## Polymorphic $\tt{ id }$

Рассмотрим самую простую полиморфную функцию — `id`:

```etude
    fun id x = x;
```

У неё есть единственное ограничение:
- тип **аргумента** `x` должен быть равен типу **возвращаемого значения** `x`.

Тогда, если `x :: $1`, то `id :: $1 -> $1`!


$$\tt{x :: a} \Rightarrow id :: a \rightarrow a$$

Но что такое `$1` (*aka* `a`)?

- Это  **неизвестный уникальный тип**,
- Он **не был ограничен** никаким конкретным типом.

Следовательно, мы **можем его обобщить**:

$$\tt{id :: \forall a \ \ a \rightarrow a}$$

## Обобщение `$1` ⇒ `G1`

**Infer Algorithm**

1. **Сгенерировать** *ограничения*,
2. **Решить** ограничения *юнификацией*,

    ![Type Store](https://raw.githubusercontent.com/otakubeam/etude-course-media/master/type_store.png)

4. **Обобщить** *свободные переменные типа*.

```
      Generalize(id.type) =
    -------------------------

            = Gen(A -> B)
          ------------------

            Gen(A)  +  Gen(B)
        -----------   ----------

            Gen($1) +  Gen($1)
          ---------   ---------

               G1   +    G1
              ----      ----
```

## [Свежие константы](https://wiki.compscicenter.ru/images/a/a0/Mlc09_2018.pdf)

## Рекурсивные функции

## Компоненты сильной связанности

**TODO;**

## Инстанцирование