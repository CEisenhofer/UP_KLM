(declare-const v0 Bool)
(declare-const v1 Bool)
(declare-const v2 Bool)
(declare-const v3 Bool)
(declare-const v4 Bool)
(assert (edge (or v0 v1) (or v2 (not v3))))
(assert (edge true (or (not v0) (not v1))))
(assert (edge (not v2) v0))
(assert (edge true (or (not v1) v2 (not v3))))
(assert (edge (or v2 v4) (not v0)))
(assert (edge (or v0 v1) (or v1 v2 (not v0))))
(assert (edge true (or v4 (not v2))))
(assert (edge (or v3 (not v0) (not v2)) true))
(assert (edge (or (not v4) (not v0)) (not v2)))
(assert (edge (or (not v3) (not v2)) (not v2)))
(assert (edge true true))
(assert (edge v3 true))
(assert (edge (or v1 (not v0)) true))
(assert (edge true (or v1 v4)))
(assert (edge (not v4) v0))
(assert (edge (or v4 (not v2)) true))
(assert (edge v3 (not v0)))
(assert (edge v3 true))
(assert (edge (or v0 v1) (or (not v2) (not v3))))
(assert (edge true true))
(assert (not (edge (or (not v0) (not v2)) (or v0 v1))))